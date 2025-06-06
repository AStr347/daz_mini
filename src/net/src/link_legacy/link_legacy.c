#define LINK_LEGACY_MODULE
#include "link_legacy_i.h"

/***
 * @brief parse received data
 * @param in_data    - [in] data to parse
 * @param in_len    - [in] data to parse length
 * @param out       - [out] out data
 * @param out_size  - [in] out data size, should be at least 1
 * 
 * @return          - total data, written in dst (0 - if no data captured)
 */
static
u32 ll_parse_received(slice_t * const in_data,
                      const u32 in_len,
                      net_rx_t* const out_data,
                      const u32 out_size)
{
    u32 receive_count = 0;

    net_rx_t * dst = out_data;

    for(uf8 sel = 0; llenc_count > sel; ++sel){
        ll_data_t rx_data;
        const b8 parse_result = legacy_parse[sel](&rx_data, in_data, in_len);

        if(likely(false == parse_result)){
            continue;
        }
        /* legacy network always return 3 bytes of extra */
        dst->id = rx_data.id;
        const u32 rdata = rx_data.data;
        const uf8 maybe_cnt = (rdata >> LLLEN_DATA_CNT_POS) & 0xff;
        if(likely(3 < maybe_cnt)){
            dst->data.cnt = 0;
        } else {
            dst->data.cnt = maybe_cnt;
            for(u8 i = 0; i < maybe_cnt; i++){
                dst->data.data[i] = (rdata >> (i << 3)) & 0xff;
            }
        }
        ++receive_count;
        if(out_size <= receive_count){
            /* no space to alloc another rx data */
            break;
        }
    }
    
    return (receive_count);
}

/**
 * @brief rx helper - return true, if something like legacy signal present 
 * @param ll		- [in] linklayer to use
 * @return		- true if something like signal present,
 * 			      false else 
 */
static 
b8 ll_check_present(THIS)
{
	slice_t* restrict data = this->slices.data;
	s32 recceive_result = this_be_receive(data,
										  LINK_LEGACY_SLICE_COUNT_MAX,
                                          20);

	if(likely(5 > recceive_result)){
        /* no signal */
        return (false);
    }
    
    recceive_result = slice_proc(data, recceive_result, 70);
    if(likely(5 > recceive_result)){
        /* no signal */
        return(false);
    }

    /* move to next high level */
    if(0 == data->level){
        ++data;
        --recceive_result;
    }

	/* make recceive result even */
	recceive_result &= 0xfffe;
	if(likely(5 > recceive_result)){
		/* too low slice count to analuse */
		return (false);
	}

	/* call the testers from pulsar/kaku/kaku_old */
    const u16 rx_threshold = (recceive_result * 3) >> 2;
	for(uf8 sel = 0;  llenc_count > sel; ++sel){
		const u16 test_res = legacy_test[sel](data, recceive_result, rx_threshold);
		if(test_res >= rx_threshold){
			return (true);
		}
	}
	
	return (false);
}

/**
 * recceive data
 * @param ll            - [inout] linklayer instance
 * @param data          - [out] recceived data buffer
 * @param data_size     - [in] received data buffer
 * 
 * @return              - count of received data
 */
static 
u32 ll_rx(THIS,
          net_rx_t* restrict const data,
          const u8 data_size)
{
    this_be_wake();
	/* recceive test block */
	const b8 signal_present = ll_check_present(this);

	if(false == signal_present){
        this_be_sleep();
		return (0);	
	}
	
	/* message part */
	const s32 rxres = this_be_receive(this->slices.data,
                                      LINK_LEGACY_SLICE_COUNT_MAX,
                                      220);
    this_be_sleep();

	slice_t * restrict slice_data = this->slices.data;
	if(unlikely(50 > rxres)){
        /* too low rx count to handle
         * any package
         */
        return (0);
    }
    
    /* we have data to parse */
    u32 normal_count = slice_proc(slice_data,
                                  rxres,
                                  70);

    /* set first symbol to high level */
    if(0 == slice_data->level){
        ++slice_data;
        --normal_count;
    }

    if(unlikely(40 > normal_count)){
        return (0);
    }
    
    /* parse received data */
    const u32 receive_count = ll_parse_received(slice_data,
                                                normal_count,
                                                data,
                                                data_size);

	return (receive_count);
}

/**
 * @brief try to transmit pulsar3 data for 10 seconds, if channel is empty
 * @param ll        - [inout] linklayer instance
 * @param id		- [in] destination identifier
 * @param data		- [in] data to transmit
 * 
 * @return          - true if success
 *                    false else
 */
static 
b8 ll_tx(THIS,
         const u64 id,
         const net_data_t * const data)
{
	b8 result = false;
	const llenc_type_t lltype = (id >> LLTYPE_OFFSET) & LLTYPE_MASK;

	if(likely(llenc_count <= lltype)){
        return result;
    }

    u8 data0 = 0 < data->cnt ? data->data[0]:0;
    u8 data1 = 1 < data->cnt ? data->data[1]:0;
    
    const ll_data_t txdata = {
        .id = id,
        .data = ((u16)data1 << 8) | data0,
    };
    const uf8 count = legacy_make[lltype](this->slices.data, LINK_LEGACY_SLICE_COUNT_MAX, &txdata);
    if(likely(count)){
        this_be_wake();
        /**
         * backend have feature TX if CCA, but pulsar3 have
         * long sync, and at sync channel withis be clear
         * and transmission will be started
         */
        result = this_be_transmit(count, this->slices.data, 30);
        this_be_sleep();
    }

	return result;
}

/***
 * @brief turn on legacy network wrapper for radio backend
 * @param ll		- [inout] linklayer instance
 */
static 
void ll_on(THIS)
{
    this_be_on();
}

/***
 * @brief turn off legacy network wrapper for radio backend
 * @param ll		- [inout] linklayer instance
 */
static 
void ll_off(THIS)
{
    this_be_off();
}

/**
 * init legacy linklayer
 * @param ll		- legacy linlayer instance
 * @param conf		- config
 */

net_legacy_be_t* link_legacy_init(THIS,
                      			  const link_legacy_conf_t * const conf)
{
    memclr(this, sizeof(link_legacy_t));
    this->link.ft = &nft_common;
    this->link.this = this;

	/* init backend */
    this->be = conf->be_init(conf->be, conf->be_conf);

	/* set target freq */
    const u32 freq = conf->freq;
	this->freq = freq;
    //this_be_freq(freq);

    return &this->link;
}
