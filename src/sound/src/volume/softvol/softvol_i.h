#ifndef SOFTVOL_MODULE
#error "must be include just into softvol.c"
#endif
#include "softvol.h"

static
void softvol_set(softvol_t * const this,
                 const u8 volume,
                 samplebuffer_t * const buff);

static const
snd_vol_ft_t softvol_ft = {
    .set = (snd_vol_set_t)softvol_set,
};
