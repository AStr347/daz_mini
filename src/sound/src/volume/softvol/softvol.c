#define SOFTVOL_MODULE
#include "softvol_i.h"

static
void softvol_set(softvol_t * const this,
                 const u8 volume,
                 samplebuffer_t * const buff)
{
    this->snd_vol.volume = volume;
    const u8 bufsel = buff->bufsel;
    u16 * dst = buff->data[bufsel];
    for(u16 i = 0; i < buff->count[bufsel]; i++){
        s32 value = *dst;
        value = (value * volume) >> 8;
        *dst++ = value;
    }
}

snd_vol_t* softvol_init(softvol_t * const this, const softvol_conf_t * const conf)
{
    this->snd_vol.this = this;
    this->snd_vol.ft = &softvol_ft;
    this->snd_vol.volume = 0;
    return &this->snd_vol;
}
