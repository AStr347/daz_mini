#ifndef RTC_DRV_H
#define RTC_DRV_H
#include "ia_arch.h"
#include "inevent.h"
#include "bcd_time.h"

#define RTC_PRIORITY                (2)
#define RTC_STACK_SIZE              (configMINIMAL_STACK_SIZE + 16)

extern
void rtc_init(inevent_t * const in0);

/**
 * get RTC counter value - we interpritate it how posix time value
 */
extern
u32 rtc_posix_get(void);

/**
 * get bcd time value from RTC
 */
extern
bcd_time_t rtc_bcd_get(void);

/**
 * set new RTC counter value - we interpritate it how posix time value
 */
extern
void rtc_posix_set(const u32 posix);

/**
 * set RTC counter by bcd time
 */
extern
void rtc_bcd_set(const bcd_time_t * const bcd);

#endif//RTC_DRV_H

#if defined(DEVICE_TREE_H)
/* nothing to extern */
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(RTC_DEVICE_TREE)
#define RTC_DEVICE_TREE
#define RTC_INIT()		rtc_init(&INE_DEFAULT)
#endif
