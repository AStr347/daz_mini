/**
 * simple 8bit fifo for common purposes
 * size should be power of two
 * 
 * this variant have sync primitives
 * and have greater perfomance 
 */
#ifndef SFIFO8_H
#define SFIFO8_H
#include "ia_arch.h"

/**
 * simple fifo8 struct
 * @mem tail		- read index
 * @mem cnt		- count in bytes
 * @mem size		- max fifo count in bytes
 * @mem mask		- rollover mask 
 * @mem data		- fifo data storage
 */
typedef struct {
	u16 tail;
	u16 cnt;
	u16 size;
	u16 mask;
	u8* restrict data;
} sfifo8_t;

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
extern b8 sfifo8_init(sfifo8_t * restrict fifo,
	       u8* restrict data,
	       const uf16 size);

/**
 * reset byte fifo
 * @param fifo		- [out] fifo to reset
 */
extern void sfifo8_reset(sfifo8_t * restrict fifo);

/**
 * put 32bit to the fifo
 * @param fifo		- [inout] fifo to add data 
 * @param data		- [in] of data to put
 * 
 * @return 		- true on success
 * 			  false if no space to put data 
 */
__attribute__((warn_unused_result))
extern b8 sfifo8_put(sfifo8_t * restrict fifo,
		const u8* restrict data,
		const u16 cnt);

/**
 * get data from the fifo
 * @param fifo		- [in] fifo instance
 * @param data		- [out] data destination 
 * @param cnt		- [in] max data count to read
 * 
 * @return 		- read data count
 */
__attribute__((warn_unused_result))
extern u16 sfifo8_get(sfifo8_t * restrict fifo,
		u8 * restrict data,
		const u16 cnt);

/**
 * return true if fifo is empty, false else
 * @param fifo		- fifo to test
 * @return 		- true if empty
 * 			  false else
 */
__attribute__((always_inline))
static inline b8 sfifo8_is_empty(sfifo8_t * restrict const fifo)
{
	return fifo->cnt? false: true;	
}

#endif
