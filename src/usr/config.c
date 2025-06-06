#include "cfg_storage.h"

T_DEVICE_CONFIG config;

__attribute__((section(".boards_config")))
const T_DEVICE_CONFIG config_in_flash = {
	.dev_common = {
		.id = 0xffffffffffffffff,
		.indcirc = 0xff,
		.long_signal = 0xff,
		.night_color = 0xff,
		.night_bri = 3,
		.vol = 0xff,

		.alarm_melody = 0xff,
		.snooze = 0xff,
	},
	.condev = EMPTY_CONNECTED_LIST,
};
