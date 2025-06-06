#ifndef TRANSISTORVOL_H
#define TRANSISTORVOL_H
#include "snd_vol.h"
#include "dport.h"

#ifndef TRANSISTORVOL_CNT
#define TRANSISTORVOL_CNT (1)
#endif

typedef struct {
    DPORT port[TRANSISTORVOL_CNT];
    u8 pin[TRANSISTORVOL_CNT];
} transistorvol_conf_t;

typedef struct {
    snd_vol_t snd_vol;
    DPORT port[TRANSISTORVOL_CNT];
    u16 mask[TRANSISTORVOL_CNT];
} transistorvol_t;

extern
snd_vol_t * transistorvol_init(transistorvol_t * this, const transistorvol_conf_t * const conf);

#endif//TRANSISTORVOL_H

#if defined(DEVICE_TREE_H)
extern transistorvol_t transistorvol;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(TRANSISTORVOL_DEVICE_TREE)
#define TRANSISTORVOL_DEVICE_TREE

transistorvol_t transistorvol;

static const
transistorvol_conf_t transistorvol_conf = {
    .port[0] = TRANSISTORVOL_PORT0,
    .pin[0] = TRANSISTORVOL_PIN0,

#if (1 < TRANSISTORVOL_CNT)
	.port[1] = TRANSISTORVOL_PORT1,
    .pin[1] = TRANSISTORVOL_PIN1,
#endif

#if (2 < TRANSISTORVOL_CNT)
	.port[2] = TRANSISTORVOL_PORT2,
    .pin[2] = TRANSISTORVOL_PIN2,
#endif

#if (3 < TRANSISTORVOL_CNT)
	.port[3] = TRANSISTORVOL_PORT3,
    .pin[3] = TRANSISTORVOL_PIN3,
#endif

#if (4 < TRANSISTORVOL_CNT)
	.port[4] = TRANSISTORVOL_PORT4,
    .pin[4] = TRANSISTORVOL_PIN4,
#endif

#if (5 < TRANSISTORVOL_CNT)
	.port[5] = TRANSISTORVOL_PORT5,
    .pin[5] = TRANSISTORVOL_PIN5,
#endif

#if (6 < TRANSISTORVOL_CNT)
	.port[6] = TRANSISTORVOL_PORT6,
    .pin[6] = TRANSISTORVOL_PIN6,
#endif

#if (7 < TRANSISTORVOL_CNT)
	.port[7] = TRANSISTORVOL_PORT7,
    .pin[7] = TRANSISTORVOL_PIN7,
#endif
};

#define SND_VOL_INIT 	transistorvol_init
#define SND_VOL 		(&transistorvol)
#define SND_VOL_CONF 	(&transistorvol_conf)
#endif
