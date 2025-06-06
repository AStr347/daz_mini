#define PULSAR_CATCHER_MODULE
#include "pulsar_catcher_i.h"

#define BAD_PRE_FIND    (0xffff)
#define BAD_PROC_DATA   (0xffffffffffffffff)

/**
 * return true, if test in range from min to max
 * @param test	- [in] test value
 * @param min	- [in] range minimum
 * @param max	- [in] range maximum
 * @return
 */
#define in_range(__test__, __min__, __max__)	((__test__ >= __min__) && (__test__ <= __max__))

/*
 * encode slen to pass into pulsar extra
 * @param slen	- [in] len to encode
 * 
 * @return 	- encoded num
 */
__attribute__((const))
static u8 slen_encode(const u16 slen)
{
	if(150 < slen){
		return (slen - 125)/50;		
	}
	return 0;
}

/*
 * decode slen to make slices
 * @param enc	- [in] encoded lenght
 * 
 * @return 	- base len
 */
__attribute__((const))
static u16 slen_decode(const u8 enc)
{
	return (150 + 50*enc);	
}

/**
 * serch pulsar preamble
 * 
 * @param data
 * @param cnt
 * @return  0xffff - preamble not found
 *          offset of data start otherwice
 */
__attribute__((noinline))
static
u16 pulsc_pre_find(const slice_t * restrict data,
                   const u16 cnt)
{
    if(P3ANY_PCNT_MIN > cnt){
        return BAD_PRE_FIND;
    }
    if(0 != data->level){
        data++;
    }
    for(u16 i = 0; i < cnt - P3ANY_PCNT_MIN; i+=2){
        const u16 pre_len = data[i].length;
        if(PRE_MIN <= pre_len){
            const u16 hi_len = data[i + 1].length;
            const u16 lo_len = data[i + 2].length;
            
            const u16 max_len = hi_len > lo_len? hi_len : lo_len;
            const u16 min_len = hi_len < lo_len? hi_len : lo_len;
            const u16 pair_len = max_len + min_len;
            if(in_range(pair_len, PAIR_MIN, PAIR_MAX)){
                const u16 div = max_len / min_len;
                /* div ~1/3 - think ok */
                if(in_range(div, 2, 4)){
                    return i + 1;
                }
            }
        }
    }
    return BAD_PRE_FIND;
}

/**
 * parce pulsar packet
 * 
 * @param data
 * @param cnt
 * @return 0xffffffffffffffff - bad packet
 *         u64 id othrewice
 */
__attribute__((noinline))
static
u64 pulsc_proc_data(const slice_t * restrict data,
                        const u16 cnt)
{
    u32 sum_llen = 0;
    u32 sum_slen = 0;
    u64 result = 0;
    u8 plen = 0;
    data += 1;
    for(u16 i = 0; i < cnt - 2; i += 2){
        const u16 hi_len = data[i].length;
        const u16 lo_len = data[i + 1].length;
        u8 bit_value = 0;
        u16 max_len = 0;
        u16 min_len = 0;
        if(hi_len > lo_len){
            bit_value = 1;
            max_len = hi_len;
            min_len = lo_len;
        } else {
            bit_value = 0;
            max_len = lo_len;
            min_len = hi_len;
        }
        if(PRE_MIN <= lo_len){
            /* it's end of msg */
            if(P3ANY_LEN_MIN <= plen){
                /* once good way */
                /* fill slen, mult pairs */
                const u32 midl_slen = sum_slen / plen;
                const u32 midl_llen = sum_llen / plen;
                const u8 enc = (slen_encode(midl_slen)) & PULS_ALFA_LEN_MASK;
                const u8 mult = ((midl_llen + midl_slen) / midl_slen);
                const u8 wmult = (mult - 2) & PULS_MULT_MASK;
                const u64 extra = ((u64)plen << LLLEN_OFFSET) | ((u64)enc << PULS_ALFA_LEN_OFFSET) | ((u64)(wmult) << PULS_MULT_OFFSET);
                result |= extra;
                /* enc or mult calculating another than pulsar_any */
                /* net_legacy eat it - ok */
                return result;
            } else {
                /* not enought data */
                return BAD_PROC_DATA;
            }
        }
        const u16 pair_len = max_len + min_len;
        if(in_range(pair_len, PAIR_MIN, PAIR_MAX)){
            /* it's pulsar data pair */
            const u16 div = max_len / min_len;
            /* div ~1/3 - think ok */
            if(in_range(div, 2, 4)){
                sum_llen += max_len;
                sum_slen += min_len;
                result <<= 1;
                result |= bit_value;
                plen++;
            } else {
                /* broken packet */
                return BAD_PROC_DATA;
            }
        }
    }
    /* cuted packet */
    return BAD_PROC_DATA;
}

/**
 * parse pulsar data
 * @param dst		- [out] parsed data destination
 * @param data		- [in] data, started from 1
 * @param cnt		- [in] datacount
 *
 * @return		- pulsar any out if success,
 * 			  zero else
 */
b8 pulsc_parse(ll_data_t* restrict dst,
               const slice_t * restrict data,
               const u16 cnt)
{
    b8 result = false;
	if(P3ANY_PCNT_MIN > cnt){
		/* minimum pulsar length is (16bits + presync + postsync)*/
		return 0;
	}
    
    /* search pre */
    u16 dcnt = cnt; // data cnt
    u64 out_data = 0;
    for(;;){
        const u16 offset = pulsc_pre_find(data, dcnt);
        if(BAD_PRE_FIND == offset){
            /* nothing like preamble */
            return false;
        }
        dcnt -= offset;
        if(P3ANY_PCNT_MIN > dcnt){
            /* data cnt less then minimal packet size */
            return false;
        }
        data += offset;
        out_data = pulsc_proc_data(data, dcnt);
        if(BAD_PROC_DATA != out_data){
            /* we parce pulsar data -> complite output and return */
            break;
        }
    }
    
    if(0 != out_data){
        dst->id = ((u64)llenc_puls << LLTYPE_OFFSET) | (out_data & DPULS_LC_AMASK);
        dst->data = (DPULS_DATA_CNT << LLLEN_DATA_CNT_POS) | ((out_data >> DPULS_LC_DPOS) & DPULS_LC_DMASK);
        result = true;
	}
    return (result);
}

/**
 * return true, if something like pulsar present in seq
 * @param data		- [in] data to analyse, first high
 * @param cnt		- [in] data count
 * 
 * @return            count of pulsar-like samples
 */
u16 pulsc_test(const slice_t * restrict data,
               const u16 cnt,
               const u16 need_cnt)
{
	u16 result = 0;
    
    for(u16 i = 0; i < cnt - 1; i+= 2){
        const u16 hi_len = data[i].length;
        const u16 lo_len = data[i + 1].length;
        
        const u16 max_len = hi_len > lo_len? hi_len : lo_len;
        const u16 min_len = hi_len < lo_len? hi_len : lo_len;
        const u16 pair_len = max_len + min_len;
        if(in_range(pair_len, PAIR_MIN, PAIR_MAX)){
            const u16 div = max_len / min_len;
            /* div ~1/3 - think ok */
            if(in_range(div, 2, 4)){
                result += 2;
                /* faster test return for not losting packets */
                if(need_cnt <= result){
                    return (result);
                }
            } else {
                result = 0;
            }
        }
    }
	return (result);
}

/**
 *
 * @param data	- [out] slices out
 * @param size	- [in] slicse size
 * @param src	- [in] data to proceed
 *
 * @return 	 slices count, or 0 if not converted
 */
u16 pulsc_make(slice_t * restrict dst, const u16 size, const ll_data_t* restrict src)
{
	uf8 len = (src->id >> LLLEN_OFFSET) & LLLEN_MASK;
	if((P3ANY_LEN_MIN > len) || (size < (len + 2))){
		return 0;
	}
	const u16 slen = slen_decode((src->id >> PULS_ALFA_LEN_OFFSET) & PULS_ALFA_LEN_MASK);
	const u8 smult = 2 + ((src->id >> PULS_MULT_OFFSET)&PULS_MULT_MASK);

	u64 to_tx = src->id & PULS3_TOTX_MASK;
	
	slice_t block0 = {.level = 1};
	slice_t block1 = {.level = 0};

	/* tx now from highest to lowes */
	u64 tx_mask = (u64)1 << (len - 1);
	for(uf8 __len = len ; __len; --__len){
		const b8 sign = 0 < (to_tx & tx_mask);
		block0.length = sign ? (slen*smult) : slen;
		*dst++ = block0;
		block1.length = sign ? slen : (slen*smult);
		*dst++ = block1;
		tx_mask >>= 1;
	}

	/* add sync */
	block0.length = slen;
	*dst++ = block0;
	block1.length = slen*31;
	*dst++ = block1;
	++len;

	return (len << 1);
}
