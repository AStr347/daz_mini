#define LIST_MODULE
#include "list_i.h"

/* local */

/**
 * not safe!
 * helper function that works with raw index 
 * @param list		- list to work with
 * @param entry		- entry to put
 * @param rindex	- raw index to put
 */
static inline void put_rindex(list_t * restrict list,
			      const void* restrict entry,
			      const u32 rindex)
{
	const u32 size = list->size;
	const void * restrict src = entry;
	void * restrict dst = list->data + list->data_offset + rindex * list->size;

	memcpy(dst, src, size);
}

/**
 * not safe!
 * @param list		- list to work with
 * @param rindex	- raw index
 * @return 		- data pointer
 */
static inline void* get_rindex(list_t * restrict list,
			       const u32 rindex)
{
	return list->data + list->data_offset + rindex * list->size;
}

/**
 * not safe!
 * translate filled index to run index
 * @param list		- list to work
 * @param index		- filled index
 * @return 		- run inex, or (u32)(-1) if wrong index is selected
 */
static inline u32 ftor_index(list_t * restrict list,
			       const u32 index)
{
	__label__ exit;
	u32 result = (u32)(-1);
	u32 num = 0;

	/* for every group */
	for(uf gsel = 0; gsel < (list->data_offset >> 2); ++gsel){
		u32 work = list->pcache[gsel];
		
		while(work){
			uf sel;

			sel = 31 - clz(work);
			work &= ~(1 << sel);
			
			if(unlikely(index == num)){
				/* ok, we found */
				result = (gsel << 5) + 31 - sel;
				goto exit;
			}
			++num;
		}
	}	
exit:	
	return result;
}

/* global */

/**
 * init list struct
 * @param list		- new list location
 * @param max		- max count
 * @param size		- size of one entry
 * @return 		- size of list block
 */
u32 list_init(list_t * restrict list,
	      const u32 max,
	      const u32 size)
{
	memclr(list, sizeof(list_t));
	/* real data offset from data start */
	const u32 data_offset = (((max + 31) >> 5) << 2);
	/* aligned size */
	const u32 nsize = (size + 3)&0xfffffffc;
	/* full block size */
	const u32 result = 12 + data_offset + max * nsize;
	/* clean list data */
	memclr(list, result);
	/* fill common list data */
	list->max = max;
	list->rsize = size;
	list->size = nsize;
	list->data_offset = data_offset;
	
	/* done */
	return result;
}

/**
 * clear all list data
 * @param list		- list to reset
 */
void list_reset(list_t * restrict list)
{
	/* clear cnt and cache */
	list->cnt = 0;
	memclr(list->pcache, list->data_offset);
}

/**
 * add entry to list
 * @param list		- list to add
 * @param entry		- entry to add
 * @return		- if >= 0 index of new entry
 * 			  if < 0, cannot allocate
 * 
 */
s32 list_add(list_t * restrict list,
	     const void* restrict entry)
{
	__label__ exit;
	s32 result = -1;

	/* check free space */
	if(likely(list->cnt + 1 > list->max)){
		/* find free */
		goto exit;
	}
	
	/* for every group */
	for(uf gsel = 0; gsel < (list->data_offset >> 2); ++gsel){
		/* found first free entry in cache line */
		u32 work = ~list->pcache[gsel];
		if(work){
			uf sel;

			sel = 31 - clz(work);
			/* ok, we found */
			list->pcache[gsel] |= 1 << sel;
			++list->cnt;
			result = (gsel << 5) + 31 - sel;
			memcpy(list->data + list->data_offset + result*list->size, entry, list->size);
			goto exit;
		}
	}
exit:	
	return result;
}

/**
 * get entry to index  
 * @param list		- list to get
 * @param dst		- location to put entry
 * @param size		- dst size
 * @param index		- index of new entry
 * @return 		- if >= 0 index of entry
 * 			  if < 0 can't get
 */
s32 list_get(list_t * restrict list,
	     void* restrict dst,
	     const u32 size,
	     const u32 index)
{
	s32 result = - 1;
	if(likely(size >= list->size && index < list->cnt)){
		const uf rindex = ftor_index(list, index);
		const void* restrict src = get_rindex(list, rindex);
		
		memcpy(dst, src, list->rsize);
		result = list->rsize;
	}
	return result;
}

/**
 * rewrite data in the location with index
 * @param list		- list to put
 * @param entry		- entry to put
 * @param index		- idex to put
 * @return		- if >= 0 index of new entry
 * 			  if < 0 cannot allocate
 */
s32 list_put(list_t * restrict list,
	     const void * restrict entry,
	     const u32 index)
{
	s32 result = -1;

	if(likely(index < list->cnt)){
		const uf rindex = ftor_index(list, index);
		void* restrict dst = get_rindex(list, rindex);
		
		memcpy(dst, entry, list->rsize);
		result = list->rsize;		
	}

	return result;
}

/**
 * del entry from list
 * @param list		- list to add
 * @param index		- entry index to del
 * @return		- if >= 0 index of deleted entry
 * 			  if < 0, cannot del
 * 
 */
s32 list_del(list_t * restrict list,
	     const u32 index)
{
	__label__ exit;
	s32 result = -1;
	u32 num = 0;

	if(unlikely(index >= list->cnt)){
		goto exit;
	}
	
	/* for every group */
	for(uf gsel = 0; gsel < (list->data_offset >> 2); ++gsel){
		u32 work = list->pcache[gsel];
		
		while(work){
			uf sel;

			sel = 31 - clz(work);
			work &= ~(1 << sel);
			
			if(unlikely(index == num)){
				/* ok, we found */
				list->pcache[gsel] &= ~(1 << sel);
				--list->cnt;
				result = index;
				goto exit;
			}
			++num;
		}
	}

	exit:
	return result;
}
