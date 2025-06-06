#ifndef CONFIG_STORAGE_H
#define CONFIG_STORAGE_H
#include "ia_arch.h"
#include "configurations.h"

enum cfg_storage_errors {
	CFG_STORAGE_SUCCESS,
	CFG_STORAGE_NOCHANGE = 1,
	CFG_STORAGE_ERAZE_ERR = 2,
	CFG_STORAGE_WRITE_ERR = 3,	
};

/**
 * init the flash storage module
 * @param	- pointer to RAM location
 * @param	- pointer to FLASH location
 * @return 	- true if success
 */
b8 cfg_storage_init(T_DEVICE_CONFIG * restrict config_ram,
		    const T_DEVICE_CONFIG * restrict config_flash);

/**
 * sync the flash storage with ram storage
 * @return 	- zero on success,
 * 		  error code else
 */
u32 cfg_storage_sync(void);

#endif /* CONFIG_STORAGE_H */
