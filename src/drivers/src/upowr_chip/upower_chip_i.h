#ifndef UPOWER_CHIP_MODULE
#error "must be include just into upower_chip.c"
#endif
#include "upower_chip.h"
#include "adc.h"

/* externed funcs */
static
upower_levels_t upower_chip_get_charge(void * this);
static
upower_stat_t upower_chip_get_stat(void * this);
static
b8 upower_chip_is_good(void * this);

/* internal func */
static
void upower_chip_monitor(void * this);
static
void upower_chip_plock(void * this);
static
void upower_chip_punlock(void * this);

static const    
upower_ft_t upower_chip_ft = {
    // .get_charge = (upower_be_get_charge_t) upower_chip_get_charge,
    // .get_stat = (upower_be_get_stat_t) upower_chip_get_stat,
    // .power_is_good = (upower_be_is_good_t) upower_chip_is_good,

    .monitor = (upower_be_monitor_t) upower_chip_monitor,
    .plock = (upower_be_plock_t) upower_chip_plock,
    .punlock = (upower_be_punlock_t) upower_chip_punlock,
};
