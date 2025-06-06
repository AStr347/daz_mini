#ifndef CONFIGURATIONS_APP_H
#define CONFIGURATIONS_APP_H
#include "ia_arch.h"
#include "notify_info.h"
#include "savedinfo.h"
#include "board.h"

/* network-specific includes */
#ifdef NET
#include "net.h"
#endif
#ifdef NET_LEGACY
#include "net_legacy.h"
#endif

/* if network present, we will have some extra functions to handle data */
#if defined(NET) || defined(NET_LEGACY)
#include "netlayer_generic.h"
/* defines some functions for pack/unpack net data */
#define NET_STUFF
#endif

/* todo: seems this macro
 * should be located somewhere else
 * 
 * phase of device testing
 * @enum DEVT_NO	- no test passed
 * @enum DEVT_BOARD	- led, vibro, etc easy board test passed,
 *	 		  also device already have mac address
 * 
 * @emum DEVT_RADIO	- device radio seems works fine
 * @enum DEVT_ALL	- all tests passed
 */
typedef enum __packed{
	DEVT_NO = 0,
	DEVT_BOARD = 1 << 0,
	DEVT_RADIO = 1 << 1,
	DEVT_ALL = (1 << 0) | (1 << 1),
} devtest_phase_t;

/**
 * contain device configuration
 * @mem dev_common		- common device configuration
 * @mem condev			- connected devices data 
 */
typedef struct {
	DEV_COMMON_T dev_common;

	struct {
		notify_storage_t info;
#ifdef NOTIFY_COUNT_MAX
		DEV_INFO_T data[NOTIFY_COUNT_MAX];
#endif
	} condev;
} T_DEVICE_CONFIG;

extern
T_DEVICE_CONFIG config;

/**
 * translate device type to indication index
 * @param devtype		- device type
 * @return 			- indication index
 */
extern
u8 conf_type_ind_index(const u8 devtype);

/**
 * check that this type of indication
 * is presetn in paired data
 * 
 * @param ind_type	- type to check
 * @return 		- true if present,
 * 			  false if not present
 */
extern
b8 conf_ind_check(T_DEVICE_CONFIG * restrict const conf,
                  const uf ind_type);

/* used for fast definition connected list in Danish devices */
#define EMPTY_CONNECTED_LIST	\
(notify_storage_t){\
    .cnt = 0,\
	.entry_size = sizeof (DEV_INFO_T),\
	.cnt_max = NOTIFY_COUNT_MAX,\
	.group_present = ((NOTIFY_COUNT_MAX - 1) >> 5) + 1,\
	.present_cache = {\
        [0] = 0,\
        [1] = 0,\
        [2] = 0,\
        [3] = 0,\
        [4] = 0,\
        [5] = 0,\
        [6] = 0,\
        [7] = 0,\
    },\
}
    
#ifdef NET_STUFF

/***
 * @brief add packed net paired data from location to conf struct
 * @param conf		- conf storage
 * @param info		- connected device info
 * 
 * @return 		- true on success
 */
extern
b8 conf_net_add(T_DEVICE_CONFIG * restrict const conf,
                const DEV_INFO_T * restrict info);


/* sub packed net paired data from location to conf struct
 * @param conf		- conf storage
 * @param mac		- mac address to remove
 * @return 		- true if removed,
 * 			  false else
 */
extern
b8 conf_net_del(T_DEVICE_CONFIG* restrict const conf,
                const u64 mac);

/** unpack paired data from config and load it to network
 * @param net		- [in] network to load paired
 * @param conf          - [in] storage of config
 * @return                      - count of extracted entries
 */
extern
u32 conf_gnet_get(gnet_t * restrict const net,
                  const T_DEVICE_CONFIG * restrict conf);

/* flush paired data from config
 * @param conf		- conf storage
 */
extern
void conf_net_flush(T_DEVICE_CONFIG * restrict const conf);

/**
 * return true if device with selected mac is present in config
 * @param conf		- config to check
 * @param mac		- mac to chec
 * @return 		- true if present,
 * 			  false else
 */
extern
b8 conf_net_present(T_DEVICE_CONFIG * restrict const conf,
                    const u64 mac);

/**
 * check network config integrity
 * if broken - flush the config
 * @param conf		- network config
 * @return		- true if all ok
 *			  false if something wrong and config was flushed
 */
extern
b8 conf_net_icheck(T_DEVICE_CONFIG * restrict const conf);

/**
 * get notify from config
 * @param conf		- [in] config to use
 * @param mac		- [in] mac address to extract
 * 
 * @return 		- pointer to data or null,
 * 			  if not present
 */
extern
DEV_INFO_T* conf_net_notify_get(T_DEVICE_CONFIG * restrict const conf,
                                const u64 mac);

#endif
#endif
