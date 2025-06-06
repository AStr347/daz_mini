#ifndef CONFIG_H
#define CONFIG_H
#include "ia_arch.h"

typedef enum __packed {
	IS_LED_P = 0,
	IS_SOUND_P = 1,
	IS_VIBRO_P = 2,

	IS_LED_M = (1 << IS_LED_P),
	IS_SOUND_M = (1 << IS_SOUND_P),
	IS_VIBRO_M = (1 << IS_VIBRO_P),

	IS_NONE = 0,
	IS_LED = IS_LED_M,
	IS_SOUND = IS_SOUND_M,
	IS_SOUND_LED = IS_SOUND_M | IS_LED_M,
	IS_VIBRO = IS_VIBRO_M,
	IS_VIBRO_LED = IS_VIBRO_M | IS_LED_M,
	IS_VIBRO_SOUND = IS_VIBRO_M | IS_SOUND_M,
	IS_ALL = IS_VIBRO_M | IS_SOUND_M | IS_LED_M,
} indcirc_t;


typedef struct __packed {
    u64 id;
    u8 color;
	u8 melody;
	u8 vibro;
	u8 reserved0;
    u8 name;
} devinfo_t;

typedef struct __packed {
    u64 id;
    indcirc_t indcirc;
    u8 long_signal;
    u8 night_color;
    u8 night_bri;

    u8 vol;
    u8 alarm_melody;
    u8 snooze;
    u8 reserved0;
} config_t;

#define DEV_COMMON_T 	config_t
#define DEV_INFO_T 		devinfo_t

#endif//CONFIG_H
