#ifndef I2S_MODULE
#error should be included to i2s_sound.c
#endif
#include "pwm_sound.h"

#define THIS pwms_entry_t * const this

/* variables for use with i2s */

static
b8 pwm_sound_on(THIS);

static
void pwm_sound_off(THIS);

static
void pwm_sound_start(THIS, const samplebuffer_t * const buff);

static
void pwm_sound_stop(THIS);

static
b8 pwm_sound_wait(THIS);

/* sound function table for i2s */
static const
snd_be_ft_t pwm_sound_ft = {
	.on = (snd_be_on_t)pwm_sound_on,
    .off = (snd_be_off_t)pwm_sound_off,
    .start = (snd_be_start_t)pwm_sound_start,
    .stop = (snd_be_stop_t)pwm_sound_stop,
    .wait = (snd_be_wait_t)pwm_sound_wait,
};
