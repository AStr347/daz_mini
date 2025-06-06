#ifndef SOUND_TONE_MODULE
#error should be included to tone.c only
#endif
#include "tone.h"
#include "trig.h"
#include "string.h"

/* tonegen module */
#define MOD         tonegen_t * restrict const this

/**
 * init tone generator
 * @param this		- [out] tonegenerator workarea
 * @param param		- [in] tone params
 * 	@mem samplerate	- [in] sound samplerate
 * 	@mem freq	- [in] target freq
 * 	@mem len	- [in] length in ms
 * 
 * @return		- true if tonegenerator was initiated
 */
static b8 tone_init(MOD, const sound_t* restrict const sound);

/**
 * tone fillbuff callback, used to fill play buffers with tone
 * @param tg		- [inout] tonegenerator descriptor
 * @param dst		- [out] buffer to fill
 * @param volume	- [in] volume level
 * 
 * @return	- true in need to play,
 * 		  false if playing done
 */
static b8 tone_fillbuff(MOD, samplebuffer_t * restrict const dst);

/**
 * close mmcwav associated file
 * @param this		- [inout] tonegenerator descriptor 
 */
static b8 tone_stop(MOD);

const gen_ft_t tone_generator = {
	.fill = (gen_fill_t)tone_fillbuff,
	.init = (gen_init_t)tone_init,
	.stop = (gen_stop_t)tone_stop,
};
