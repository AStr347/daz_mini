/* 
 * File:   snd_engine.h
 * Author: dron
 *
 * Created on 19 10 2017 ., 15:52
 */

#ifndef SND_ENGINE_H
#define	SND_ENGINE_H
#include "ia_arch.h"
#include "sfifo8.h"
#include "inevent.h"
#include "board.h"
#include "snd_gen.h"
#include "snd_be.h"
#include "snd_vol.h"

#define SND_STACK_SIZE          		(BASE_STACK_SIZE)
#define SOUND_COMMAND_BUFFER_SIZE       32
/* Signal buff can used by modules what we need to know snd events */
#define SIZE_OF_SIGNAL_BUFF             5

/**
 * states of snd machine
 * @enum SND_STAT_IDLE		- sound is idle
 * @enum SND_STAT_PLAYING	- sound is playing
 */
typedef enum __packed {
	SND_STAT_IDLE,
	SND_STAT_PLAYING,
	SND_STAT_COUNT,
} sound_stat_t;

typedef struct {
    snd_be_init_t be_init;
    void * const be;
    const void * const be_conf;

	snd_vol_init_t snd_vol_init;
	void * const snd_vol;
    const void * const snd_vol_conf;

	inevent_t* restrict input;
} snd_conf_t;

typedef struct snd_entry {
	inevent_t* restrict input;
	snd_be_t * be;
	snd_vol_t * vol;
	snd_gen_t gen;

	const snd_conf_t * restrict conf;
	
	volatile sound_stat_t stat;
	sound_codec_t codec;
	u8 volume;
	
    /* routine work area */
    struct {
        u8 sound_block_cnt;
        u8 play_cnt;
        u8 silence;
        b8 have_data;
        const sound_t * restrict sound;
    } rwa;
	
	sfifo8_t combuf;
	u8 combuf_data[SOUND_COMMAND_BUFFER_SIZE];

	tdm_t mutex;
	tdc_t cond;
	TASK_T(SND_STACK_SIZE);
} snd_entry_t;

typedef void (*snd_stat_handler_t)(snd_entry_t * restrict const this);

typedef void (*snd_coms_handlers_t)(snd_entry_t * restrict const this);

typedef void (*snd_stops_t)(snd_entry_t * restrict const this);

/**
 * Initialize sound engine
 * @param sound_entry - entry for sound engine
 * @param backend - backend (driver) for sound engine
 * @return SUCCESS if succes,  otherwise e_sound_engine_status_error
 */
extern
void snd_engine_init(snd_entry_t * restrict const this,
					 const snd_conf_t * restrict const snd_conf);

/* interface */

/**
 * start playing melody
 * @param snd		- [inout] sound device
 * @param sound		- [in] sound to play
 * 
 * @return 		- true if success,
 * 			  false else
 */
extern
b8 snd_sound_play(snd_entry_t * restrict const this,
		  		  const sound_t * restrict const sound,
		  		  const u8 play_cnt);

/**
 * stop playing melody
 * @param snd		- sound device
 * @return 		- true if stop req generated
 */
extern
b8 snd_sound_stop(snd_entry_t * restrict const this);

/**
 * set new volume
 * @param snd		- sound device
 * @param new_volume	- new volume
 * @return 		- true if success, false else
 */
extern
b8 snd_volume_set(snd_entry_t * restrict const this,
		  		  const u8 new_volume);

#endif

#if defined(DEVICE_TREE_H)
extern snd_entry_t snd0;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(SND_DEVICE_TREE)
#define SND_DEVICE_TREE

#if !defined(SND_BE_INIT) || !defined(SND_BE) || !defined(SND_BE_CONF) ||\
	!defined(SND_VOL_INIT) || !defined(SND_VOL) || !defined(SND_VOL_CONF)
#error "sound backend thing not defined"
#endif

snd_entry_t snd0;
static const
snd_conf_t snd_conf0 = {
	.be_init = (snd_be_init_t) SND_BE_INIT,
    .be = SND_BE,
    .be_conf = SND_BE_CONF,

	.snd_vol_init = (snd_vol_init_t) SND_VOL_INIT,
	.snd_vol = SND_VOL,
    .snd_vol_conf = SND_VOL_CONF,

	.input = &INE_DEFAULT,
};
#define SND_INIT()		snd_engine_init(&snd0, &snd_conf0)
#endif
