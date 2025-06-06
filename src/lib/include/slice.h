#ifndef SLICE_H
#define SLICE_H

#include "ia_arch.h"

/**
 * slice with level and value
 * @mem	raw	- raw data
 * @mem level	- low/high level
 * @mem length	- slice length 
 */

typedef union __attribute__((__transparent_union__)) {
	u16 raw;
	struct {
		u16 length : 15;
		u16 level : 1;/* must be highest bit */
	};
} slice_t;

#define SLICE_NOT_DETECTED	(u16)(-1)
#define SLICE_LEN_MAX		(0x7fff)

/**
 * glue slices in seq beside with one sign and too short slices
 * @param data		- [inout] source and destination of normalize
 * @param size		- [in] target size, must be more then one
 * @param cutoff	- [in] slice minimum length, if less slice will be cutted off
 * 
 * @return 		- new target size
 */
extern u32 slice_proc(slice_t * restrict data,
	       const u32 size,
	       const u16 cutoff);
#endif


