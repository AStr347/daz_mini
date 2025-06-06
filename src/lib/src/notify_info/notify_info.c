/**
 * module to store net info
 * for simple devices
 */
#define MODULE_NOTIFY_INFO
#include "notify_info_internal.h"

/**
 * select coffin with index in storage
 * @param storage		- storage to use
 * @param index			- coffin index
 */
static inline
coffin_t* select_coffin(const notify_storage_t * restrict const storage,
				        const u32 index)
{
	return (coffin_t*)((u32)storage->data + (index * storage->entry_size));
}

/**
 * @brief get notify info from storage
 * @param storage		- [in] storage to get
 * @param mac			- [in] mac to get
 * 
 * @return              - pointer to notify info,
 *                        or null if not present
 */
coffin_t* notify_get(const notify_storage_t * restrict const storage,
                     const u64 id)
{
	const u32 group_count = storage->group_present;

	for(uf sel = 0; sel < group_count; ++sel){
		u32 work = storage->present_cache[sel];
		for(; work;){
			const uf16 selected = 31 - clz(work);
			coffin_t * restrict sel_data = select_coffin(storage, selected + (sel << 5));
/* #warning may be need masking how in net?
 * egan 09 sep 2023 - think, this can be done on upper levels:
 * Want to keep this module simple as possible.
 */
			if(unlikely(id == sel_data->id)){
				return sel_data;
			}
			work &= ~(1 << selected);
		}
	}

	return NULL;
}

/**
 * put notify info to storage
 * @param storage		- storage to use
 * @param dev_info		- device info to add
 * @return 			- true on success
 * 				  false if not enough space to add
 */
b8 notify_put(notify_storage_t * restrict const storage,
	      const coffin_t * restrict const dev_info)
{
	const u32 group_count = storage->group_present;
	const u64 id = dev_info->id;

	/* try to get the data from storage */
	coffin_t * restrict coffin = notify_get(storage, id);
	if(NULL != coffin){
		/* we found new entry - fill the new entry with new data
		 * we do not need to copy the mac address
		 */
		memcpy(coffin, dev_info, storage->entry_size);
		return true;
	}

	/* ok, we don't found the entry in the list
	 * try to find empty space
	 */

	if(unlikely(storage->cnt >= storage->cnt_max)){
		/* not enough space */
		return false;
	}

	/* find new place to put the data */
	for(uf sel = 0; sel < group_count; ++sel){
		coffin_t * restrict sel_data;
		uf selected;
		const u32 work = ~storage->present_cache[sel];

		if(0 != work){
			selected = 31 - clz((work)&(0 - work));
			storage->present_cache[sel] |= 1 << selected;
			sel_data = select_coffin(storage, selected + (sel << 5));
			memcpy(sel_data, dev_info, storage->entry_size);
			++storage->cnt;
			return true;
		}
	}
	/* how we get this point? */
	return false;
}

/**
 * del entry with specifie mac from storage
 * @param storage	- storage
 * @param mac		- mac address
 * @return 		- true if removed
 * 			  false if nothing happened 
 */
b8 notify_del(notify_storage_t * restrict const storage,
	      const u64 id)
{
	const u32 group_count = storage->group_present;

	for(uf sel = 0; sel < group_count; ++sel){
		u32 work = storage->present_cache[sel];
		for(; work;){
			coffin_t * restrict sel_data;
			uf selected;
			selected = 31 - clz(work);

			sel_data = select_coffin(storage, selected + (sel << 5));
			if(unlikely(id == sel_data->id)){
				storage->present_cache[sel] &= ~(1 << selected);
				--storage->cnt;
				memclr(sel_data, storage->entry_size);
				return true;
			}
			work &= ~(1 << selected);
		}
	}
	return false;
}

/**
 * get notify info from storage
 * @param storage		- storage to get
 * @param index 		- index of entry
 * @return 			- pointer to notify info,
 * 				  or null if not present
 */
coffin_t* notify_get_index(const notify_storage_t * restrict const storage,
			   const u32 index)
{
	const u32 group_count = storage->group_present;
	u32 pos = index;

	if(unlikely(index >= storage->cnt)){
		/* we have not item with this number */
		return NULL;
	}

	for(uf sel = 0; sel < group_count; ++sel){
		u32 work = storage->present_cache[sel];

		/* fast path to next cache if cache is full and num is not in
		 * the first block
		 * */
		if(unlikely(0xffffffff == work && 31 < pos)){
			pos -= 32;
			continue;
		}

		for(; work;){
			coffin_t * restrict sel_data;
			uf selected;

			selected = 31 - clz((work)&(0 - work));
			if(0 == pos){
				sel_data = select_coffin(storage, selected + (sel << 5));
				return sel_data;
			}else{
				--pos;
				work &= ~(1 << selected);
			}
		}
	}
	
	/* how we get this point ?*/
	return NULL;
}

/**
 * delete notify info from storage
 * @param storage		- storage to get
 * @param index 		- index of entry
 * @return 			- true if deleted,
 * 				  false else
 */
b8 notify_del_index(notify_storage_t * restrict const storage,
		    const u32 index)
{
	const u32 group_count = storage->group_present;
	u32 pos = index;

	if(unlikely(index >= storage->cnt)){
		/* we have not item with this number */
		return false;
	}

	for(uf sel = 0; sel < group_count; ++sel){
		u32 work = storage->present_cache[sel];

		/* fast path to next cache if cache is full and num is not in
		 * the first block
		 * */
		if(unlikely(0xffffffff == work && 31 < pos)){
			pos -= 32;
			continue;
		}

		for(; work;){
			coffin_t * restrict sel_data;
			uf selected;

			selected = 31 - clz((work)&(0 - work));
			if(unlikely(0 == pos)){
				--storage->cnt;
				storage->present_cache[sel] &= ~(1 << sel);
				/* extra clear field with data */
				sel_data = select_coffin(storage, selected + (sel << 5));
				memclr(sel_data, storage->entry_size);
				return true;
			}else{
				--pos;
				work &= ~(1 << selected);
			}
		}
	}

	return false;
}

/**
 * flush all notify data
 * @param storage		- storage to flush
 */
void notify_flush(notify_storage_t * restrict storage)
{
	storage->cnt = 0;
	/* clean present cache */
	storage->present_cache[0] = 0;
	storage->present_cache[1] = 0;
	storage->present_cache[2] = 0;
	storage->present_cache[3] = 0;
	storage->present_cache[4] = 0;
	storage->present_cache[5] = 0;
	storage->present_cache[6] = 0;
	storage->present_cache[7] = 0;
	/**
	 * todo:
	 * storage now don't know about it's size!
	 * should be reworked later
	 * 10 - from macro from savedinfo.h
	 */
	memclr(&storage->data, storage->entry_size * storage->cnt_max);
}

/**
 * proceed notify integrity check
 * @param storage	- storage instance
 * @return		- zero if storage seems to be correct
 *			  error code else
 */
u32 notify_icheck(const notify_storage_t * restrict storage,
		  const u32 cnt_max)
{
	__label__ exit;
	u32 result = NSTORAGE_OK;
	const u32 group_count = storage->group_present;

	/* check max count */
	if(unlikely(cnt_max != storage->cnt_max)){
		/* bad max count */
		result = NSTORAGE_CNTM;
		goto exit;
	}

	/* check count - it should be less then max count */
	if(unlikely(cnt_max < storage->cnt)){
		/* bad count */
		result = NSTORAGE_CNT;
		goto exit;
	}

	/* check grout count */
	{
		const u32 gcount = (0 < cnt_max) ? (((cnt_max - 1) >> 5) + 1) : 0;
	
		if(unlikely(group_count != gcount)){
			/* bad group count */
			result = NSTORAGE_CNT;
			goto exit;
		}
	}

	/* check present cache - it should contain num of ones
	 * same that value we have in cnt field 
	 */
	{
		u32 sum = 0;

		for(uf sel = 0; sel < group_count; ++sel){
			u32 cache = storage->present_cache[sel];

			if(0xffffffff == cache){
				sum += 32;
				continue;
			}
			
			for(uf sel = 0; sel < cnt_max; ++sel){
				if(cache & 0x01){
					++sum;
				}
				cache >>= 1;
			}
		}

		if(unlikely(sum != storage->cnt)){
			/* bad cache */
			result = NSTORAGE_MASK;
			goto exit;
		}
	}

	/* all ok */
exit:
	return result;
}

/**
 * reset notigy storage to initial values
 * @param storage		- storage to reset
 * @param entry_size		- size of one entry
 * @param cnt			- cnt_max
 */
void notify_reset(notify_storage_t * restrict storage,
		  const u32 entry_size,
		  const u32 cnt)
{
	storage->cnt = 0;
	storage->cnt_max = cnt;
	storage->entry_size = entry_size;
	storage->group_present = (0 < cnt) ? ((cnt - 1) >> 5) + 1 : 0;
	/* clean present cache */
	storage->present_cache[0] = 0;
	storage->present_cache[1] = 0;
	storage->present_cache[2] = 0;
	storage->present_cache[3] = 0;
	storage->present_cache[4] = 0;
	storage->present_cache[5] = 0;
	storage->present_cache[6] = 0;
	storage->present_cache[7] = 0;
	/* clear the entry area */
	memclr(storage->data, cnt * entry_size);
}
