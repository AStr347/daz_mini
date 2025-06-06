/* ************************************************************************** */
/** melodies buzzer */
/* ************************************************************************** */
#ifndef BUZZER_H
#define BUZZER_H
#include "ia_arch.h"
#include "snd_engine.h"

#define NOTE_COUNT 108
#define NOTE_BEG 11
#define NOTE_END 100
#define NOTE_LEN_COUNT 13
#define NOTE_LEN_BEG 1
#define NOTE_LEN_END 9

/* used for set the max volume in device */
#define BUZZER_MAX_VOLUME	0xff
/* used for mute the device */
#define BUZZER_MUTE		0

/* buzzer names */
typedef enum  __packed {
	BUZ_FIRST = 0,
	BUZ_TEST = 0,
	BUZ_BABY,
	BUZ_DOOR,
	BUZ_DOOR2,
	BUZ_INTERCOM,
	BUZ_PHONE_HOME,
	BUZ_PHONE_CELL,
	BUZ_SMOKE,
	BUZ_WINDOW,
	BUZ_WATER,
	BUZ_HELP,
	BUZ_GAS,
	BUZ_COUNT,
	BUZ_ALARM_FIRST = BUZ_COUNT,
	BUZ_ALARM1 = BUZ_COUNT,
	BUZ_ALARM2,
	BUZ_ALARM3,
	BUZ_ALARM_COUNT,
	
	BUZ_ON,
	BUZ_OFF,
	BUZ_ALL_COUNT,
} buzzer_name_t;


/**
 * play buzzer with default subsystem
 * @param name 		- melody name, is enum
 * @param cnt		- play cnt, zero is until stop
 */
extern
void buzzer_play(const buzzer_name_t name,
                 const u8 cnt);

/**
 *  stop the buzzer
 */
extern
void buzzer_stop(void);

/**
 * set default volume
 * @param volume	- volume to set
 */
extern
void buzzer_volume_set(const u8 volume);

#endif
