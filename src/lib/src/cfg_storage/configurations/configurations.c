/**
 * tight tangle of pasta with
 * various config-related code 
 */
#include "configurations.h"
#include "netlayer_generic.h"
#include "device_tree.h"
//#include "random.h"

/* global */

#ifdef NET_STUFF

/**
 * check network config integrity
 * if broken - flush the config
 * @param conf		- network config
 * @return		- true if all ok
 *			  false if something wrong and config was flushed
 */
b8 conf_net_icheck(T_DEVICE_CONFIG * restrict const conf)
{
	b8 result = true;
    const u32 check = notify_icheck(&conf->condev.info, NOTIFY_COUNT_MAX);
    const b8 check_res = (NSTORAGE_OK != check);
	if(unlikely(check_res)){
		/* we got error code but always only reset storage */
		notify_reset(&conf->condev.info,
                     sizeof (DEV_INFO_T),
                     NOTIFY_COUNT_MAX);
		result = false;
	}
	return result;
}

/***
 * @brief add packed net paired data from location to conf struct
 * @param conf		- conf storage
 * @param info		- connected device info
 * 
 * @return 		- true on success
 */
b8 conf_net_add(T_DEVICE_CONFIG * restrict const conf,
                const DEV_INFO_T * restrict info)
{
	return notify_put(&conf->condev.info, (coffin_t*)(info));
}

/**
 * get notify from config
 * @param conf		- [in] config to use
 * @param mac		- [in] mac address to extract
 * 
 * @return 		- pointer to data or null,
 * 			  if not present
 */
DEV_INFO_T* conf_net_notify_get(T_DEVICE_CONFIG * restrict const conf,
				const u64 mac)
{
	return (DEV_INFO_T*)notify_get(&conf->condev.info, mac);
}

/**
 * return true if device with selected mac is present in config
 * @param conf		- config to check
 * @param id		- id to chec
 * 
 * @return          - true if present,
 *                    false else
 */
b8 conf_net_present(T_DEVICE_CONFIG * restrict const conf,
                    const u64 id)
{
	if(NULL != notify_get(&conf->condev.info, id)){
		return true;
	}else{
		return false;
	}
}

/* sub packed net paired data from location to conf struct
 * @param conf		- conf storage
 * @param id		- id to remove
 * 
 * @return          - true if removed,
 *                    false else
 */
b8 conf_net_del(T_DEVICE_CONFIG * restrict const conf,
                const u64 id)
{
	return notify_del(&conf->condev.info, id);
}

        /*** common network iface ***/

/** 
 * @brief unpack paired data from config and load it to network
 * @param net		- [in] network to load paired
 * @param conf          - [in] storage of config
 * 
 * @return                      - count of extracted entries
 */
u32 conf_gnet_get(gnet_t * restrict const net,
                  const T_DEVICE_CONFIG * restrict conf)
{
	const u32 cnt = conf->condev.info.cnt;
    u32 addcnt = 0;

	/* load network settings */
    const u8 type = net->type;  /* networks type */
    /* if id field is empty, random will be used*/
    const u64 base_id = 0 < conf->dev_common.id ? conf->dev_common.id : (0x11 | random32());
    /* address conversion */
    const u64 target_id = NETLAYER_TYPE == type ?\
                          (base_id & (u64)0xffffffff):\
                          ((u64)0xffff000000000000 & base_id) ? base_id : (base_id | NETL_BASE_ID_INFO);
    
	net_mac_set(net, target_id);

	/* add connected devices */
	for(uf8 sel = 0; cnt > sel; ++sel){
		DEV_INFO_T * restrict const ent = (DEV_INFO_T*)notify_get_index(&conf->condev.info, sel);
		if(NULL != ent){
            const u8 etype = ID_TO_TYPE(ent->id);
            
            if(etype == type){
                ++addcnt;
            }
		}
	}

	return addcnt;
}

/* flush paired data from config
 * @param conf		- conf storage
 */

void conf_net_flush(T_DEVICE_CONFIG * restrict conf)
{
	return notify_flush(&conf->condev.info);
}

#endif
