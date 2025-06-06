#ifndef BCB0802_H
#define BCB0802_H
#include "ia_arch.h"
#include "graphic_be.h"
#include "dport.h"

#define GRAPHIC_BE_ROW_CNT      (2)
#define GRAPHIC_BE_COLUMN_CNT   (8)

typedef enum {
    BCB0802_EXTS_BAT_EMPTY = 0,
    BCB0802_EXTS_BAT_LOW,
    BCB0802_EXTS_BAT_MID,
    BCB0802_EXTS_BAT_FULL,

    BCB0802_EXTS_CNT,
} bcb0802_extsymb_t;

typedef struct {
    DPORT rs_port;
    DPORT rw_port;
    DPORT e_port;
    DPORT power_port;
    DPORT d_ports[8];

    u8 rs_pin;
    u8 rw_pin;
    u8 e_pin;
    u8 power_pin;
    u8 d_pins[8];
} bcb0802_conf_t;

typedef struct {
    graphic_be_t be;

    DPORT rs_port;
    DPORT rw_port;
    DPORT e_port;
    DPORT power_port;
    DPORT d_ports[8];

    u16 rs_mask;
    u16 rw_mask;
    u16 e_mask;
    u16 power_mask;
    u16 d_masks[8];

    u8 screen[GRAPHIC_BE_ROW_CNT][GRAPHIC_BE_COLUMN_CNT];
    u8 expose[GRAPHIC_BE_ROW_CNT];

    b8 powered;
} bcb0802_t;


extern
graphic_be_t * bcb0802_init(bcb0802_t * const this, const bcb0802_conf_t * const conf);


#endif//BCB0802_H

#if defined(DEVICE_TREE_H)
extern bcb0802_t bcb0802;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(BCB0802_DEVICE_TREE)
#define BCB0802_DEVICE_TREE

bcb0802_t bcb0802;

static
const bcb0802_conf_t bcb0802_conf = {
    .rs_port = BCB0802_RS_PORT,
    .rw_port = BCB0802_RW_PORT,
    .e_port = BCB0802_E_PORT,
    .power_port = BCB0802_POWER_PORT,
    .d_ports = {
        [0] = BCB0802_D0_PORT,
        [1] = BCB0802_D1_PORT,
        [2] = BCB0802_D2_PORT,
        [3] = BCB0802_D3_PORT,
        [4] = BCB0802_D4_PORT,
        [5] = BCB0802_D5_PORT,
        [6] = BCB0802_D6_PORT,
        [7] = BCB0802_D7_PORT,
    },

    .rs_pin = BCB0802_RS_PIN,
    .rw_pin = BCB0802_RW_PIN,
    .e_pin = BCB0802_E_PIN,
    .power_pin = BCB0802_POWER_PIN,
    .d_pins = {
        [0] = BCB0802_DO_PIN,
        [1] = BCB0802_D1_PIN,
        [2] = BCB0802_D2_PIN,
        [3] = BCB0802_D3_PIN,
        [4] = BCB0802_D4_PIN,
        [5] = BCB0802_D5_PIN,
        [6] = BCB0802_D6_PIN,
        [7] = BCB0802_D7_PIN,
    },
};

#define GRAPHIC_BE_INIT     ((graphic_be_init_t)bcb0802_init)
#define GRAPHIC_BE          ((void *)&bcb0802)
#define GRAPHIC_BE_CONF     ((const void * const)&bcb0802_conf)
#endif
