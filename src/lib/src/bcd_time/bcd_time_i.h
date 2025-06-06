#ifndef BCD_TIME_I_H
#define BCD_TIME_I_H

#include "bcd_time.h"

/* Nonzero if YEAR is a leap year (every 4 years,
   except every 100th isn't, and every 400th is).  */
#define __isleap(__YEAR__) \
    ((__YEAR__) % 4 == 0 && ((__YEAR__) % 100 != 0 || (__YEAR__) % 400 == 0))


typedef struct {
	u16 tm_sec;   /* Seconds.	    [0-59] */
    u16 tm_min;   /* Minutes.	    [0-59] */
    u16 tm_hour;  /* Hours.	        [0-23] */
    u16 tm_mday;  /* Day.		    [1-31] */
    u16 tm_mon;   /* Month.	        [0-11] */
    u16 tm_year;  /* Year           from 1970 */
    u16 tm_wday;  /* Day of week.	[0-6] */
    u16 tm_yday;  /* Days in year.	[0-365] */
} tm_t;

/**
 * convert unix time to tm_t
 * 
 * @param t - [in] unix time
 * @param tp - [out] struct for fill
*/
extern
void __localtime32(const u32 t, tm_t *tp);

/**
 * convert tm_t to unix time
 * @param t - [in] struct for convert
*/
extern
u32 __mktime32(const tm_t * const t);

#endif//BCD_TIME_I_H