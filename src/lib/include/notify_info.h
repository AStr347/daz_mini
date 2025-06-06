#ifndef NOTIFY_INFO_H
#define NOTIFY_INFO_H
#include "ia_arch.h"

/**
 * return values for integrity check
 * @enum NSTORAGE_OK		- no errors
 * @enum NSTORAGE_CNT		- bad count value
 * @enum NSTORAGE_CNTM		- bad max count
 * @enum NSTORAGE_MASK		- present mask value
 */
enum {
	NSTORAGE_OK = 0,
	NSTORAGE_CNT = 1,
	NSTORAGE_CNTM = 2,
	NSTORAGE_MASK = 3,
};

/**
 * contain mac address and unspecified info
 */
typedef struct coffin {
	u64 id;			/* uniq access descriptor */
	u8 data[0];		/* data */
} coffin_t;

/**
 * contain all notify info
 * @mem cache of connected devices
 * each bit declares that connected_devices[bit index] is present  
 * @mem device info
 * @mem group_present		- max count of groups in storage,
 *                            cnt_max >> 5
 * @mem present_cache		- every bit of storage is represent
 *                            one present entry
 */
typedef struct {
	u16 cnt;
	u16 entry_size;
	u16 cnt_max;
	u16 group_present;
	u32 present_cache[8];
	coffin_t data[0];
} notify_storage_t;

/**
 * get notify info from storage
 * @param storage		- storage to get
 * @param mac			- mac to get
 * @return 			- pointer to notify info,
 * 				  or null if not present
 */
coffin_t* notify_get(const notify_storage_t * restrict const storage,
		     const u64 id);

/**
 * put notify info to storage
 * @param storage		- storage to use
 * @param dev_info		- device info to add
 * @return 			- true on success
 * 				  false if not enough space to add
 */
b8 notify_put(notify_storage_t * restrict const storage,
	      const coffin_t * restrict const dev_info);

/**
 * del entry with specifie mac from storage
 * @param storage	- storage
 * @param mac		- mac address
 * @return 		- true if removed
 * 			  false if nothing happened 
 */
b8 notify_del(notify_storage_t * restrict const storage,
	      const u64 id);

/**
 * get notify info from storage
 * @param storage		- storage to get
 * @param index 		- index of entry
 * @return 			- pointer to notify info,
 * 				  or null if not present
 */
coffin_t* notify_get_index(const notify_storage_t * restrict const storage,
			   const u32 index);

/**
 * delete notify info from storage
 * @param storage		- storage to get
 * @param index 		- index of entry
 * @return 			- true if deleted,
 * 				  false else
 */
b8 notify_del_index(notify_storage_t * restrict const storage,
		    const u32 index);

/**
 * flush all notify data
 * @param storage		- storage to flush
 */
void notify_flush(notify_storage_t * restrict storage);

/**
 * proceed notify integrity check
 * @param storage	- storage instance
 * @return		- zero if storage seems to be correct
 *			  error code else
 */
u32 notify_icheck(const notify_storage_t * restrict storage,
		  const u32 cnt_max);

/**
 * reset notigy storage to initial values
 * @param storage		- storage to reset
 * @param entry_size		- size of one entry
 * @param cnt			- cnt_max
 */
void notify_reset(notify_storage_t * restrict storage,
		  const u32 entry_size,
		  const u32 cnt);

/**
 * get count of busy entries in storage
 * @param storage	- storage
 * @return 		- count of entries
 */
__attribute__((always_inline))
static inline uf16 notify_cnt_get(const notify_storage_t* restrict storage)
{
	return storage->cnt; 
}

#endif
