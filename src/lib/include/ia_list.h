#ifndef IA_LIST_H
#define IA_LIST_H
#include "ia_arch.h"
/* list struct 
 * @mem cnt		- count of entries
 * @mem max		- entries max
 * @mem size		- entry size
 * @mem pcache		- entries present cache
 * @mem data		- raw data block
 */
typedef struct list {
	u16 cnt;
	u16 max;
	u16 size;
	u16 rsize;
	u32 data_offset;
	u32 pcache[0];
	u8 data[0];
} list_t;

/**
 * init list struct
 * @param list		- new list location
 * @param max		- max count
 * @param size		- size of one entry
 * @return 		- size of list block
 */
u32 list_init(list_t* restrict list,
	      const u32 max,
	      const u32 size);

/**
 * clear all list data
 * @param list		- list to reset
 */
void list_reset(list_t* restrict list);

/**
 * add entry to list
 * @param list		- list to add
 * @param entry		- entry to add
 * @return		- if >= 0 index of new entry
 * 			  if < 0, cannot allocate
 * 
 */
s32 list_add(list_t* restrict list,
	     const void* restrict entry);

/**
 * get entry to index  
 * @param list		- list to get
 * @param dst		- location to put entry
 * @param size		- dst size
 * @param index		- index of new entry
 * @return 		- if >= 0 index of entry
 * 			  if < 0 can't get
 */
s32 list_get(list_t* restrict list,
	       void* restrict dst,
	       const u32 size,
	       const u32 index);

/**
 * put data to to location with index 
 * @param list		- list to put
 * @param entry		- entry to put
 * @param index		- idex to put
 * @return		- if >= 0 index of new entry
 * 			  if < 0 cannot allocate
 */
s32 list_put(list_t* restrict list,
	    const void * restrict entry,
	      const u32 index);

/**
 * del entry from list
 * @param list		- list to add
 * @param index		- entry index to del
 * @return		- if >= 0 index of new entry
 * 			  if < 0, cannot allocate
 * 
 */
s32 list_del(list_t* restrict list,
	     const u32 index);

#endif//IA_LIST_H