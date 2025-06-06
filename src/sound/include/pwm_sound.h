#ifndef PWM_SOUND_H
#define PWM_SOUND_H
#include "ia_arch.h"
#include "snd_be.h"
#include "pwm.h"

typedef struct {
	pwm_t * const pwm;
	u8 pwm_ch;
} pwms_config_t;

/**
 * @mem sft		- sound function table 
 * @mem int_res		- interrupt resource
 * @mem dma_channel	- dma channel number to use with i2s module
 * @mem cond		- ?
 * @mem done_cond	- done condition for interrupt processing
 * 
 * @mem play_brg    - baud rate divisor for play operations
 * @mem rec_brg     - baud rate divisor for record operations
 * @mem mode        - current mode/stat of snd_engine(rec, play, idle)
 */
typedef struct {
	snd_be_t be;
	pwm_t * pwm;
	u8 pwm_ch;
	snd_be_mode_t mode;
} pwms_entry_t;

extern
snd_be_t * pwm_sound_init(pwms_entry_t * const this, const pwms_config_t * const conf);

#endif//PWM_SOUND_H

#if defined(DEVICE_TREE_H)
extern pwms_entry_t pwm_sound;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(PWM_SOUND_DEVICE_TREE)
#define PWM_SOUND_DEVICE_TREE

pwms_entry_t pwm_sound;
static const
pwms_config_t pwm_sound_conf0 = {
	.pwm = PWM_SOUND_HW,
	.pwm_ch = PWM_SOUND_HW_CH,
};
#define SND_BE_INIT 	pwm_sound_init
#define SND_BE 			(&pwm_sound)
#define SND_BE_CONF 	(&pwm_sound_conf0)
#endif
