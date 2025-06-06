#ifndef SOUND_GENERATOR_H
#define SOUND_GENERATOR_H
#include "ia_arch.h"
/* contain commons for sound play sample generators */

#define SND_GEN_MAX_SIZE 	(32)

/*todo: strange question about buffet size */
#define SAMPLEBUFFER_SIZE           	(1024)      /* 1 buf if 1/16 = 64 ms */
#define HALFBUFFER_SIZE                 (SAMPLEBUFFER_SIZE >> 1)

/* samblebuffer struct
 * @mem bufsel  - selected buffer, can be 0 or 1
 * @mem count	- count of sampes in buffer
 * @mem data	- payload
 */
typedef struct samplebuffer {
    u32 bufsel; // u32 for data normal aligned
	u16 count[2];
	s16 data[2][HALFBUFFER_SIZE];
} samplebuffer_t;

/**
 * types of sound codecs
 */
typedef enum __packed {
	SND_CODEC_NONE = 0,
	SND_CODEC_TONE,
	SND_CODEC_CNT,
} sound_codec_t;

/**
 * defines playing sound
 * @mem codec	- compression method
 * @mem len	- length in bytes
 * @mem data	- data
 */
typedef struct sound {
	struct {
		sound_codec_t codec : 8;
		unsigned len : 24;
	};
	u8 data[0];
} sound_t;

/**
 * init generator
 * @param gen		- [out] generator workarea
 * @param param		- [in] generator parameters
 * 
 * @return		- true, if init was done successfully
 */
typedef b8 (*gen_init_t) (void * restrict const gen, const sound_t* restrict const param);

/**
 * fill buffer with generator
 * @param gen		- [inout] generator workarea
 * @param dst		- [out] buffer to fill
 * @param volume	- [in] volume level
 * 
 * @return 		- true if something to play present
 */
typedef b8 (*gen_fill_t) (void * restrict const gen, samplebuffer_t * restrict const dst);

/**
 * free generator resources
 * @param gen		- [inout] generator
 */
typedef void (*gen_stop_t)(void * restrict const gen);

/**
 * sample generator function table
 * @mem init	- init generator
 * @mem fill	- fill play buffer
 */
typedef struct genft {
	gen_init_t init;
	gen_fill_t fill;
	gen_stop_t stop;
} gen_ft_t;

typedef struct {
	const gen_ft_t * ft;
	u8 codec_workarea[SND_GEN_MAX_SIZE];
	samplebuffer_t pbuf;
} snd_gen_t;

#define this_gen_init(__PARAM__)\
(this->gen.ft->init(this->gen.codec_workarea, __PARAM__))

#define this_gen_fill()\
(this->gen.ft->fill(this->gen.codec_workarea, &this->gen.pbuf))

#define this_gen_stop(__PARAM__)\
(this->gen.ft->stop(this->gen.codec_workarea))

#endif
