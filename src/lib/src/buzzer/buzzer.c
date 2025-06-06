#define BUZZER_MODULE
#include "buzzer_i.h"

/**
 * play buzzer with default subsystem
 * @param name 		- melody name, is enum
 * @param cnt		- play cnt, zero is until stop
 */
void buzzer_play(const buzzer_name_t name,
                 const u8 cnt)
{
	/* we will not use buzzer play for play until stop feature */
	if (likely(name < BUZ_ALL_COUNT) && (0 < cnt)) {
		snd_sound_play(&snd0, buzzer_melodies[name], cnt);
	}
}

/**
 *  stop the buzzer
 */
void buzzer_stop(void)
{
	snd_sound_stop(&snd0);
}

/**
 * set default volume
 * @param volume	- volume to set
 */
void buzzer_volume_set(const u8 volume)
{
	if(volume >= sizeof(volume_list5)){
		return;
	}
	const u8 volume_raw = volume_list5[volume];
	snd_volume_set(&snd0, volume_raw);
}
