#include "bcd_time_i.h"

/**
 * return count of leap days from 1970 to year0
*/
static
u32 leap_days(const u32 year0){
	u16 leap = 0;
	for(u16 y = 0; y < year0; y++){
		leap += __isleap(TM_YEAR_BASE + y);
	}
	return leap;
}

/**
 * convert unix time to tm_t
 * 
 * @param t - [in] unix time
 * @param tp - [out] struct for fill
*/
void __localtime32(const u32 t, tm_t *tp)
{
	/* time part - just div's */
	u32 time = t;
	tp->tm_sec = time % 60;
	time /= 60;
	tp->tm_min = time % 60;
	time /= 60;
	tp->tm_hour = time % 24;
	time /= 24;

    u32 days = time;
	/* year calculate by sub's loop */
	u16 y = 0;
	u16 ydaymax = __isleap(TM_YEAR_BASE + y)? 366 : 365;
	while(days >= ydaymax){
		days -= ydaymax;
		y++;
		ydaymax = __isleap(TM_YEAR_BASE + y)? 366 : 365;
	}
	tp->tm_year = y;
	tp->tm_yday = days;

	/* month calculate by sub's loop */
	u16 month = 0;
	u16 dmax = bcdt_month_daycount_by_values(y, month + 1);
	u16 d = days;
	while(d >= dmax){
		d -= dmax;
		month++;
		dmax = bcdt_month_daycount_by_values(y, month + 1);
	}
	tp->tm_mon = month;

	/* mday(day in month) is [1; 31] */
	const u8 day = d + 1;
	tp->tm_mday = day;
	tp->tm_wday = bcdt_dayofweek_by_values(TM_YEAR_BASE + y, month + 1, day);
}

/**
 * convert tm_t to unix time
 * @param t - [in] struct for convert
*/
u32 __mktime32(const tm_t * const t)
{
	u32 time = t->tm_sec;
	time += t->tm_min * 60;
	time += t->tm_hour * 3600;
	time += t->tm_yday * 86400;
	const u16 y = t->tm_year;
	/* just not leap year */
	time += y * 31536000;
	/* calculate cnt of leap and add */
	const u16 leap = leap_days(y);
	time += leap * 86400;
	return time;
}
