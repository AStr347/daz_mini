#define CFG_STORAGE_MODULE
#include "cfg_storage.h"
#include "string.h"
#include "device_tree.h"


/* pointer for flash and ram storage */
T_DEVICE_CONFIG * restrict cfg_ram;
const T_DEVICE_CONFIG * restrict cfg_flash;

/**
 * init the flash storage module
 * @param	- pointer to RAM location
 * @param	- pointer to FLASH location
 * @return 	- true if success
 */
#ifdef __DEBUG
volatile u32 conf_siz = sizeof(T_DEVICE_CONFIG);
volatile u32 dev_siz = sizeof(DEV_INFO_T);
#endif
b8 cfg_storage_init(T_DEVICE_CONFIG * restrict config_ram,
		    const T_DEVICE_CONFIG * restrict config_flash)
{
	b8 result = false;
#ifdef __DEBUG
    if(conf_siz >= (1024 * 8)){
        for(;;){
            __asm__ volatile ("nop");
        }
    }
#endif

	if(likely((config_ram) && (config_flash))){
		cfg_ram = config_ram;
		cfg_flash = config_flash;
		memcpy(config_ram, config_flash, sizeof (T_DEVICE_CONFIG));
		result = true;
	}

	return result;
}


/**
 * sync the flash storage with ram storage
 * @return 	- zero on success,
 * 		  error code else
 */
u32 cfg_storage_sync(void)
{
	return 0;
}
