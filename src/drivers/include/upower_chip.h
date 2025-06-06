#ifndef UPOWER_CHIP_H
#define UPOWER_CHIP_H
#include "upower_be.h"
#include "dport.h"
#include "adc.h"

typedef struct {
    DPORT chg_port;
    DPORT stat_port;
    DPORT adc_port;

    u8 chg_pin;
    u8 stat_pin;
    u8 adc_pin;
    u8 adc_ch;
} upower_chip_conf_t;

typedef struct {
    upower_be_t be;

    DPORT chg_port;
    DPORT stat_port;

    u16 chg_mask;
    u16 stat_mask;
    u8 adc_ch;
} upower_chip_t;

extern
upower_be_t * upower_chip_init(upower_chip_t * const this, const upower_chip_conf_t * const conf);


#endif//UPOWER_CHIP_H

#if defined(DEVICE_TREE_H)
extern upower_chip_t upower_chip;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(UPOWER_CHIP_DEVICE_TREE)
#define UPOWER_CHIP_DEVICE_TREE

upower_chip_t upower_chip;

static
const upower_chip_conf_t upower_chip_conf = {
    .chg_port = UPOWER_CHIP_CHRG_PORT,
    .stat_port = UPOWER_CHIP_STAT_PORT,
    .adc_port = UPOWER_CHIP_ADC_PORT,

    .chg_pin = UPOWER_CHIP_CHRG_PIN,
    .stat_pin = UPOWER_CHIP_STAT_PIN,
    .adc_pin = UPOWER_CHIP_ADC_PIN,
    .adc_ch = UPOWER_CHIP_ADC_CH,
};

#define UPOWER_BE_INIT     ((upower_be_init_t)upower_chip_init)
#define UPOWER_BE          ((void *)&upower_chip)
#define UPOWER_BE_CONF     ((const void * const)&upower_chip_conf)
#endif
