#ifndef BCD_TIME_H
#define BCD_TIME_H
#include "ia_arch.h"
#include "bcd.h"

/**
 * !!!!!!!!!!!!
 * all pair of min - max work like
 * min <= value < max
 * !!!!!!!!!!!!
*/

#define MIN_SEC			(0)
#define MAX_SEC			(60)
#define MIN_SEC_BCD		(0x00)

#define MIN_MIN			(0)
#define MAX_MIN			(60)
#define MIN_MIN_BCD		(0x00)

#define MIN_HOUR		(0)
#define MAX_HOUR		(24)
#define MIN_HOUR_BCD	(0x00)

#define MIN_DAY			(1)
#define MAX_DAY			(32)
#define MIN_DAY_BCD		(0x01)

#define MIN_MONTH		(1)
#define MAX_MONTH		(13)
#define MIN_MONTH_BCD	(0x01)

#define MIN_YEAR		(24)
#define MAX_YEAR		(99)
#define MIN_YEAR_BCD	(0x24)

#define TM_YEAR_BASE    (1970)
#define BCD_YEAR_BASE   (2000)

#define BASE_YEAR_SUB	(BCD_YEAR_BASE - TM_YEAR_BASE)
#define TM_MIN_CONVERT	(BASE_YEAR_SUB + MIN_YEAR)

#define BCDDY2TM(__YEAR__)   ((BCD_YEAR_BASE + (__YEAR__)) - TM_YEAR_BASE)

/* convert tm year to bcdd year, minimal bcdd year is MIN_YEAR */
#define TMY2BCDD(__YEAR__)   (((__YEAR__) < TM_MIN_CONVERT)? (MIN_YEAR) : ((TM_YEAR_BASE + (__YEAR__)) - BCD_YEAR_BASE))

/* bcd time and date struct */
typedef struct __packed
{
	union {
		struct {
			u8 sec;
			u8 min;
			u8 hour;
			u8 reserved0;
		};
		u32 raw_time;
	};
	union {
		struct {
			u8 day;
			u8 month;
			u8 year;
			u8 weekday;
		};
		u32 raw_date;
	};
} bcd_time_t;


extern
bcd_time_t bcd;


/***
 * calculete day of week
 * @param year  > 1752 !!! real year 2023 not 0x23 or 23 or 53
 * @param month 1 ... 12
 * @param day
 * @return day of week
 */
extern
__attribute__((const))
u8 bcdt_dayofweek_by_values(const u32 year, const u8 month, const u8 day);

/***
 * calculete day of week by bcd
 * @return day of week
 */
extern
__attribute__((const))
u8 bcdt_dayofweek(const bcd_time_t * restrict const bcd);

/**
 * calculate count of days in month
 * simplified verion without mod 100 and mod 400 checking
 * @param year		- selected year !!! tm_t format 2023 = 0x53
 * @param month		- selected month !!! january is 1
 * @return  		- count of days in the selected month
 * 			- or 0 if wrong values is passed
 */
extern
__attribute__((const))
u8 bcdt_month_daycount_by_values(const u32 year, const u8 month);

/***
 * calculete day of week by bcd
 * @return day of week
 */
extern
__attribute__((const))
u8 bcdt_month_daycount(const bcd_time_t * restrict const bcd);

/**
 * translate from posix to bcd
 * @param posix time
 * @return bcd time
 */
extern
__attribute__((const))
bcd_time_t bcdt_posix2bcd(const u32 posix);

/**
 * translate bcd to posix
 * @param bcd time
 * @return posix time
 */
extern
__attribute__((const))
u32 bcdt_bcd2posix(const bcd_time_t * restrict const bcd);

/**
 * set incorrect data to 0
 * 
 * @param bcd
 */
extern
void bcdt_fix(bcd_time_t * restrict const bcd);

#endif
