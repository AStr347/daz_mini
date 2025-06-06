/**
 * simple 8bit fifo for common purposes
 * size should be power of two
 * 
 * this variant have sync primitives
 * and have greater perfomance
 * 
 * egan2021 
 */
#define SFIFO8_MODULE
#include "sfifo8_i.h"

/**
 * init bute fifo with initial values
 * no checking of input values
 * 
 * @param fifo		- [out] fifo to init
 * @param data		- [in] pointer to fifo workarea(should be 4B aligned)
 * @param size		- [in] max size to allocate sfifo
 * 
 * @return 		- true if success
 */
b8 sfifo8_init(sfifo8_t * restrict fifo,
	       u8* restrict data,
	       const uf16 size)
{
	/* calculate correct buffer size */
	uf16 sel_size = 0;
	uf16 check_size = 2;
	for(uf8 sel = 0; 14 > sel; ++sel){
		if(size >= check_size){
			sel_size = check_size;				
		}
		check_size <<= 1;
	}

	if(likely(0 < size)){
		fifo->cnt = 0;
		fifo->tail = 0;
		fifo->mask = sel_size - 1;
		fifo->size = sel_size;
		fifo->data = data;
		return true;
	}else{
		return false;		
	}
}

/**
 * reset byte fifo
 * @param fifo		- [out] fifo to reset
 */
void sfifo8_reset(sfifo8_t * restrict fifo)
{
	fifo->cnt = 0;
	fifo->tail = 0;
}

/**
 * put 32bit to the fifo
 * @param fifo		- [inout] fifo to add data 
 * @param data		- [in] of data to put
 * 
 * @return 		- true on success
 * 			  false if no space to put data 
 */
__attribute__((warn_unused_result))
b8 sfifo8_put(sfifo8_t * restrict fifo,
		const u8* restrict data,
		const u16 cnt)
{
	const uf16 totalcnt = fifo->cnt + cnt;
	if(likely(totalcnt <= fifo->size)){
		const uf16 mask = fifo->mask;
		uf16 pos = (fifo->tail + fifo->cnt) & mask;
		const u8* restrict src = data;
		u8* restrict dst = fifo->data;
		fifo->cnt = totalcnt;
		
		/* put data */
		for(uf16 sel = cnt; sel; --sel){
			dst[pos] = *src;
			++src;
			pos = (pos + 1) & mask;
		}
		
		return true;
	}else{
		return false;
	}
}

/**
 * get data from the fifo
 * @param fifo		- [in] fifo instance
 * @param data		- [out] data destination 
 * @param cnt		- [in] max data count to read
 * 
 * @return 		- read data count
 */
__attribute__((warn_unused_result))
u16 sfifo8_get(sfifo8_t * restrict fifo,
		u8 * restrict data,
		const u16 cnt)
{
	uf16 readcnt = fifo->cnt >= cnt? cnt: fifo->cnt;
	fifo->cnt -= readcnt;
	uf16 pos = fifo->tail;
	const uf16 mask = fifo->mask;
	const uf16 result = readcnt;
	const u8* restrict src = fifo->data;
	u8* restrict dst = data;

	for(;readcnt;--readcnt){
		*dst = src[pos];
		++dst;
		pos = (pos + 1) & mask; 
	}
	fifo->tail = pos;
	
	return result;
}
