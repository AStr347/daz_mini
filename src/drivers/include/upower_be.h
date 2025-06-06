#ifndef UPOWER_BE_H
#define UPOWER_BE_H
#include "ia_arch.h"

typedef enum __attribute__((packed)){
    UPOWERS_OFF = 0b00,
    UPOWERS_CHARGING = 0b01,
    UPOWERS_CHARGED = 0b10,
    UPOWERS_UNPLUGED = 0b11,
} upower_stat_t;

typedef enum __attribute__((packed)){
    UPOWERL_NEED_OFF = 0,
    UPOWERL_LOW = 0x3f,
    UPOWERL_NORMAL = 0x7e,
    UPOWERL_PREFULL = 0x99,
    UPOWERL_FULL = 0xff,

    UPOWERL_NEED_OFF_P = 0,
    UPOWERL_LOW_P,
    UPOWERL_NORMAL_P,
    UPOWERL_PREFULL_P,
    UPOWERL_FULL_P,
    UPOWERL_LEVEL_COUNT,
} upower_levels_t;

/* externed funcs */
// typedef upower_levels_t (*upower_be_get_charge_t)(void * this);
// typedef upower_stat_t (*upower_be_get_stat_t)(void * this);
// typedef b8 (*upower_be_is_good_t)(void * this);

/* internal func */
typedef void (*upower_be_monitor_t)(void * this);
typedef void (*upower_be_plock_t)(void * this);
typedef void (*upower_be_punlock_t)(void * this);

typedef struct
{
    // upower_be_get_charge_t get_charge;
    // upower_be_get_stat_t get_stat;
    // upower_be_is_good_t power_is_good;

    upower_be_monitor_t monitor;
    upower_be_plock_t   plock;
    upower_be_punlock_t punlock;
} upower_ft_t;

typedef struct {
    const upower_ft_t * ft;
    void * this;

    upower_levels_t charge;
    upower_stat_t stat;
} upower_be_t;

typedef upower_be_t * (*upower_be_init_t)(void * const be, const void * const be_conf);

extern const
upower_levels_t upower_pos2level[UPOWERL_LEVEL_COUNT];

#endif//UPOWER_BE_H