#ifndef SND_MODULE
#error should be included to snd_engine.c only
#endif
#include "snd_engine.h"
#include "string.h"
#include "tone.h"

#define SND_DEFAULT_VOLUME		0x80
#define SND_PRIORITY			9

#define THIS snd_entry_t * restrict const this

/**
 * defines snd commands
 * @enum SND_NOOP	- no command
 * @enum SND_PLAY	- play the melody
 * @enum SND_STOP	- stop playing
 * @enum SND_VOLUME	- set the new volume
 */
enum snd_commands {
	SND_NOOP = 0,
	SND_PLAY,
	SND_STOP, 
	SND_VOLUME,
	SND_COM_COUNT,
};

extern const snd_stat_handler_t snd_stat_handlers[SND_STAT_COUNT];

extern const snd_coms_handlers_t snd_coms_handlers[SND_COM_COUNT];

extern const snd_stops_t snd_stops[SND_STAT_COUNT];

/**
 * zero all routine work area, and clear pbuf
 * 
 * @param this
 */
extern void snd_clear(snd_entry_t * restrict this);

#ifdef SND_STATIC_DATA
/* codec array */
static const
gen_ft_t * const generator_list[SND_CODEC_CNT] = {
	[SND_CODEC_TONE] = &tone_generator,
};

#endif
