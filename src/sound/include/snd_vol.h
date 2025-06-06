#ifndef SND_VOL_H
#define SND_VOL_H
#include "ia_arch.h"
#include "snd_gen.h"

/**
 * set new volume level
 * give buffer for posible softvol control
 */
typedef void (*snd_vol_set_t)(void * this,
                              const u8 value,
                              samplebuffer_t * const buff);

typedef struct {
    snd_vol_set_t set;
} snd_vol_ft_t;

typedef struct {
    const snd_vol_ft_t * ft;
    void * this;
    u8 volume;
} snd_vol_t;

typedef snd_vol_t * (*snd_vol_init_t)(void * const snd_vol, const void * const snd_vol_conf);

#ifdef SND_MODULE

#define this_vol_set(__VAL__, __BUFF__)\
(this->vol->ft->set(this->vol->this, __VAL__, __BUFF__))

#endif



#endif//SND_VOL_H
