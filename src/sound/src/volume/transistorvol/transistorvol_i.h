#ifndef TRANSISTORVOL_MODULE
#error "must be include just into transistorvol.c"
#endif
#include "transistorvol.h"

static
void transistorvol_set(transistorvol_t * const this,
                 const u8 volume,
                 samplebuffer_t * const buff);

static const
snd_vol_ft_t transistorvol_ft = {
    .set = (snd_vol_set_t)transistorvol_set,
};
