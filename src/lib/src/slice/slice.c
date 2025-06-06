#define SLICE_MODULE
#include "slice_i.h"

/**
 * glue slices in seq beside with one sign
 * @param data		- [inout] source and destination of normalize
 * @param size		- [in] target size, must be more then one
 * @param cutoff	- [in] slice minimum length, if less slice will be cutted off
 * 
 * @return 		- new target size
 */
static u32 slice_normal(slice_t * restrict data,
			const u32 size)
{
	slice_t cur = *data;
	slice_t * restrict sel = data + 1;
	slice_t * restrict dst = data;
	uf16 dsiz = 0;
	b8 needwrite = false;

	for(uf16 left = size; left; --left, ++sel){
		if(likely(cur.level != sel->level)){
			/* writeback */
			*dst++ = cur;
			cur = *sel;
			++dsiz;
			needwrite = false;
		}else{
			/* clue */
			cur.length += sel->length;
			needwrite = true;
		}
	}

	if(true == needwrite){
		*dst++ = cur;
		++dsiz;
	}

	/* null-terminated */
	dst->raw = 0;

	return dsiz;
}

/**
 * glue slices in seq with too short length
 * @param data		- [inout] source and destination of normalize
 * @param size		- [in] target size, must be more then one
 * @param cutoff	- [in] slice minimum length, if less slice will be cutted off
 * 
 * @return 		- new target size
 */
static u32 slice_filter(slice_t * restrict data,
			const u32 size,
			const u16 cutoff)
{
	slice_t cur = *data;
	slice_t * restrict sel = data + 1;
	slice_t * restrict dst = data;
	uf16 dsiz = 0;
	b8 needwrite = false;

	for(uf16 left = size; left; --left, ++sel){
		if(likely(cutoff < sel->length)){
			/* writeback */
			*dst++ = cur;
			cur = *sel;
			++dsiz;
			needwrite = false;
		}else{
			/* clue */
			cur.length += sel->length;
			needwrite = true;
		}
	}

	if(true == needwrite){
		*dst++ = cur;
		++dsiz;
	}

	/* null-terminated */
	dst->raw = 0;

	return dsiz;
}

/**
 * glue slices in seq beside with one sign and too short slices
 * @param data		- [inout] source and destination of normalize
 * @param size		- [in] target size, must be more then one
 * @param cutoff	- [in] slice minimum length, if less slice will be cutted off
 * 
 * @return 		- new target size
 */
u32 slice_proc(slice_t * restrict data,
	       const u32 size,
	       const u16 cutoff)
{
	u32 res = slice_filter(data, size, cutoff);
	return slice_normal(data, res);
}
