#ifndef SOUND_TONE_H
#define SOUND_TONE_H
#include "ia_arch.h"
#include "snd_gen.h"

/**
 * simple tone call frame
 * @mem freq - freq to play
 * @mem length - length in ms
 */
typedef struct tone {
	u16 freq;
	u16 length;
} tone_t;

/**
 * descriptor, used to make tones
 * @mem length 	- tone is length in samples of common tone
 * 		- fade is length in samples of faded tone
 * @mem count	- if this value is not zero, than it's possible to play something
 * 
 * @mem fade	- start is start amp of current block
 * 		- end is end amp of current block
 * 
 * @mem phase_adj	- used to fast calculation of samples	
 */
typedef struct {
	struct {
		u16 fadein;     /* start tone rising */
		u16 tone;
		u16 fadeout;
	} length;
	
 	struct {
		u16 start;
		u16 adj;
	} fadein;
    
	struct {
		u16 start;
		u16 adj;
	} fadeout;
	
	u16 phase;
	u16 phase_adj;

	
	u16 samplerate;
	u8 pitch;
	u8 tonecount;
	const tone_t * tone;
} tonegen_t;

extern const gen_ft_t tone_generator;

#endif
