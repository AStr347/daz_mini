#define TRANSISTORVOL_MODULE
#include "transistorvol_i.h"

#define TRANSISTORVOL_STEP  (0xff / TRANSISTORVOL_CNT)

static
void transistorvol_set(transistorvol_t * const this,
                 const u8 volume,
                 samplebuffer_t * const buff)
{
    this->snd_vol.volume = volume;
    for(u8 i = 0; i < TRANSISTORVOL_CNT; i++){
        const u8 level = (volume > (TRANSISTORVOL_STEP * i));
        DPORT_WRITE(this->port[i], this->mask[i], level);
    }
}

snd_vol_t* transistorvol_init(transistorvol_t * const this, const transistorvol_conf_t * const conf){
    for(u8 i = 0; i < TRANSISTORVOL_CNT; i++){
        this->port[i] = conf->port[i];
        this->mask[i] = (1 << conf->pin[i]);
        DPORT_DOUT_INIT(this->port[i], this->mask[i]);
        DPORT_CLR(this->port[i], this->mask[i]);
    }

    this->snd_vol.this = this;
    this->snd_vol.ft = &transistorvol_ft;
    this->snd_vol.volume = 0;
    return &this->snd_vol;
}
