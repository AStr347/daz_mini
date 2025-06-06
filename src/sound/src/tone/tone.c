/* tone generator for sound subsustem */
#define SOUND_TONE_MODULE
#include "tone_i.h"

#define OFFSETTED(__VAL__) 		( (__VAL__ >> 7) + 0x100 )

/**
 * update tone generator
 * @param this		- [out] tonegenerator workarea
 * 
 * @return		- true if updated, false else
 */ 
static
b8 tone_update(MOD)
{
	if(0 < this->tonecount){
		const tone_t* tone = this->tone;
		const u16 freq = (tone->freq * (256 + ((u32)this->pitch << 5))) >> 8;
		const u16 len = tone->length;
		const u16 samplerate = this->samplerate;
	
		const uf32 total_cnt = (len * samplerate) >> 10;  /* here some arith opt's */
		uf32 fade_out_cnt = samplerate >> 3;
        const uf32 fade_in_cnt = samplerate >> 5;

		if(likely(total_cnt >= fade_out_cnt + fade_in_cnt)){
			/* tone and fade */
			this->length.tone = total_cnt - fade_out_cnt - fade_in_cnt;
			this->length.fadeout = fade_out_cnt;
            this->length.fadein = fade_in_cnt;
		}else{
			/* all fade */
            this->length.fadein = 0;
			this->length.tone = 0;
			this->length.fadeout = fade_out_cnt = total_cnt;
		}

		/* fill adj */
		this->phase = 0;
		this->phase_adj = (TRIG_PERIOD * freq) / samplerate;
        
        this->fadein.start = 0;
		this->fadein.adj = 0xffff / fade_in_cnt;
        
		this->fadeout.start = 0xffff;
		this->fadeout.adj = 0xffff / fade_out_cnt;
        
		++this->tone;
		--this->tonecount;
		return true;
	}
	return false;
}

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
static
b8 tone_init(MOD,
		     const sound_t* restrict const sound)
{
    memclr(this, sizeof(tonegen_t));
	/* copy params */
	this->samplerate = 32000;
	this->pitch = 0xe0;
	this->tone = (tone_t*)(&sound->data);
	this->tonecount = sound->len;

	return tone_update(this);
}

/**
 * @brief fill tone fillin
 * @param dst   - [out] destinatio
 * @param siz   - [in] size of buffer
 * 
 * @return      - count of filled samples
 */
static
u16 tone_fill_fadein(MOD,
                     s16 * restrict const dst,
                     const u16 siz)
{
    const u16 fillcnt = this->length.fadein;
    const u16 to_fill = fillcnt > siz ? siz : fillcnt;
    
    /* phase part */
    uf16 phase = this->phase;
    const uf16 phase_adj = this->phase_adj;
    s32 amp = this->fadein.start;
	const s32 amp_adj = this->fadein.adj;
 
    s16 * restrict __dst = dst;
    
	for(uf16 sel = 0; to_fill > sel; ++sel){
        /* fadein filling */
        s32 value = sin16(phase);
        value = (value * amp) >> 16;

        *__dst++ = OFFSETTED(value);
        
        /* rollover seems is totally ok */
        phase += phase_adj;
        phase &= TRIG_PERIOD_M;
        amp += amp_adj;
    }
    
    this->fadein.start = (u16)amp;
    this->phase = phase;
    this->length.fadein -= to_fill;
    
    return to_fill;
}

/**
 * @brief fill tone
 * @param dst   - [out] destinatio
 * @param siz   - [in] size of buffer
 * 
 * @return      - count of filled samples
 */
static
u16 tone_fill_tone(MOD,
                   s16 * restrict const dst,
                   const u16 siz)
{
    const u16 fillcnt = this->length.tone;
    const u16 to_fill = fillcnt > siz ? siz : fillcnt;
    
    /* phase part */
    uf16 phase = this->phase;
    const uf16 phase_adj = this->phase_adj;
 
    s16 * restrict __dst = dst;
    
	for(uf16 sel = 0; to_fill > sel; ++sel){
        /* fadein filling */
        const s32 value = sin16(phase);

        *__dst++ = OFFSETTED(value);
        
        /* rollover seems is totally ok */
        phase += phase_adj;
        phase &= TRIG_PERIOD_M;
    }
    this->phase = phase;
    this->length.tone -= to_fill;
    
    return to_fill;
}

/**
 * @brief fill tone fadeout (with fadein, tone and fadeout)
 * @param dst   - [in] size of buffer
 * @param siz   - [in] size of buffer
 * 
 * @return      - count of filled samples
 */
static
u16 tone_fill_fadeout(MOD,
                      s16 * restrict const dst,
                      const u16 siz)
{
    const u16 fillcnt = this->length.fadeout;
    const u16 to_fill = fillcnt > siz ? siz : fillcnt;
    
    /* phase part */
    uf16 phase = this->phase;
    const uf16 phase_adj = this->phase_adj;
    s32 amp = this->fadeout.start;
	const s32 amp_adj = this->fadeout.adj;
 
    s16 * restrict __dst = dst;
    
	for(uf16 sel = 0; to_fill > sel; ++sel){
        /* fadein filling */
        s32 value = sin16(phase);
        value = (value * amp) >> 16;

        *__dst++ = OFFSETTED(value);
        
        /* rollover seems is totally ok */
        phase += phase_adj;
        phase &= TRIG_PERIOD_M;
        amp -= amp_adj;
    }
    
    this->fadeout.start = (u16)amp;
    this->phase = phase;
    this->length.fadeout -= to_fill;
    
    return to_fill;
}

/**
 * @breif tone fillbuff callback, used to fill play buffers with tone
 * @param tg		- [inout] tonegenerator descriptor
 * @param dst		- [out] buffer to fill
 * @param volume	- [in] volume level
 * 
 * @return	- true in need to play,
 * 		  false if playing done
 */
static
b8 tone_fillbuff(MOD,
				 samplebuffer_t * restrict const dst)
{
    /* need to be checked */
    u16 to_fill = HALFBUFFER_SIZE;
    const u8 sel = dst->bufsel;
    /* buffer to fill */
    s16 * restrict __dst = dst->data[sel];
    
    do{
        const u16 fadein = this->length.fadein;
        const u16 tone = this->length.tone;
        const u16 fadeout = this->length.fadeout;
        
        if(unlikely((0 == fadein) && (0 == tone) && (0 == fadeout))){
            const b8 update_res = tone_update(this);
            if(false == update_res){
                /* fill buffer tail with zeros */
                while(to_fill){
                    --to_fill;
                    *__dst++ = 0;
                }
                
                dst->count[dst->bufsel] = HALFBUFFER_SIZE;
                return false;
            }
        }
        
        if((0 < to_fill) && (0 < fadein)){
            const u16 fillcnt = tone_fill_fadein(this, __dst, to_fill);
            __dst += fillcnt;
            to_fill -= fillcnt;
        }
        
        if((0 < to_fill) && (0 < tone)){
            const u16 fillcnt = tone_fill_tone(this, __dst, to_fill);
            __dst += fillcnt;
            to_fill -= fillcnt;
        }
        
        if((0 < to_fill) && (0 < fadeout)){
            const u16 fillcnt = tone_fill_fadeout(this, __dst, to_fill);
            __dst += fillcnt;
            to_fill -= fillcnt;
        }
    }while(to_fill);

    dst->count[dst->bufsel] = HALFBUFFER_SIZE;
    return true;
}

/**
 * close mmcwav associated file
 * @param this		- [inout] tonegenerator descriptor 
 * 
 * @return		- always true
 */
static
b8 tone_stop(MOD)
{
	memclr(this, sizeof(tonegen_t));
	return true;	
}
