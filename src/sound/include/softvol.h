#ifndef SOFTVOL_H
#define SOFTVOL_H
#include "snd_vol.h"

typedef struct {} softvol_conf_t;

typedef struct {
    snd_vol_t snd_vol;
} softvol_t;

extern
snd_vol_t* softvol_init(softvol_t * const this, const softvol_conf_t * const conf);

#endif//SOFTVOL_H

#if defined(DEVICE_TREE_H)
extern softvol_t softvol;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(SOFTVOL_DEVICE_TREE)
#define SOFTVOL_DEVICE_TREE

softvol_t softvol;

#define SND_VOL_INIT 	softvol_init
#define SND_VOL 		(&softvol)
#define SND_VOL_CONF 	(NULL)
#endif