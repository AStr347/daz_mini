#include "bcd_time_i.h"

bcd_time_t bcd;

/***
 * calculete day of week
 * @param year  > 1752 !!! real year 2023 not 0x23 or 23 or 53
 * @param month 1 ... 12
 * @param day
 * @return day of week
 */
__attribute__((const))
u8 bcdt_dayofweek_by_values(const u32 year, const u8 month, const u8 day)
{
	static const u8 t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	const u32 _year = month < 3? year - 1 : year;
	return ((_year + (_year >> 2) - _year / 100 + _year / 400 + t[month - 1] + day) % 7);
}

/***
 * calculete day of week by bcd
 * @return day of week
 */
__attribute__((const))
u8 bcdt_dayofweek(const bcd_time_t * restrict const bcd)
{
	const u8 year = bcd8_to_bin8(bcd->year);
	const u8 month = bcd8_to_bin8(bcd->month);
	const u8 day = bcd8_to_bin8(bcd->day);
	return bcdt_dayofweek_by_values(BCD_YEAR_BASE + year, month, day);
}

/**
 * calculate count of days in month
 * simplified verion without mod 100 and mod 400 checking
 * @param year		- selected year !!! tm_t format 2023 = 0x53
 * @param month		- selected month !!! january is 1
 * @return  		- count of days in the selected month
 * 			- or 0 if wrong values is passed
 */
__attribute__((const))
u8 bcdt_month_daycount_by_values(const u32 year, const u8 month)
{
	static const u8 month_daycount[] = {0,
					31, 28, 31, 30, 31, 30,
					31, 31, 30, 31, 30, 31};
	u8 result;

	result = 0;
	if((month) && (month < MAX_MONTH)){
		result = month_daycount[month];
		if((month == 2) && (__isleap(TM_YEAR_BASE + year))){
			++result;
		}
	}
	return result;
}

/***
 * calculete day of week by bcd
 * @return day of week
 */
__attribute__((const))
u8 bcdt_month_daycount(const bcd_time_t * restrict const bcd)
{
	const u8 year = BCDDY2TM(bcd8_to_bin8(bcd->year));
	const u8 month = bcd8_to_bin8(bcd->month);
	return bcdt_month_daycount_by_values(year, month);
}

/**
 * translate from posix to bcd
 * @param posix time
 * @return bcd time
 */
__attribute__((const))
bcd_time_t bcdt_posix2bcd(const u32 posix)
{
	tm_t conv;
	__localtime32(posix, &conv);
	u16 year = TMY2BCDD(conv.tm_year);
	if(year > MAX_YEAR){
		while (year > MAX_YEAR)	{
			year -= 100;
		}
		year = MIN_YEAR + year;
	}

	const bcd_time_t result ={
		.sec = bin8_to_bcd8(conv.tm_sec),
		.min = bin8_to_bcd8(conv.tm_min),
		.hour = bin8_to_bcd8(conv.tm_hour),
		.day = bin8_to_bcd8(conv.tm_mday),
		.month = bin8_to_bcd8(conv.tm_mon + 1),
		.year = bin8_to_bcd8(year),
		.weekday = conv.tm_wday,
	};

	return result;
}

/**
 * translate bcd to posix
 * @param bcd time
 * @return posix time
 */
__attribute__((const))
u32 bcdt_bcd2posix(const bcd_time_t * restrict const bcd)
{
	const u8 day = bcd8_to_bin8(bcd->day);
	const u8 month = bcd8_to_bin8(bcd->month) - 1;
	const u16 y = BCDDY2TM(bcd8_to_bin8(bcd->year));
	u16 d = day - 1;
	for (u8 m = 0; m < month; m++){
		d += bcdt_month_daycount_by_values(y, m + 1);
	}
	
	const tm_t conv = {
		.tm_sec = bcd8_to_bin8(bcd->sec),
		.tm_min = bcd8_to_bin8(bcd->min),
		.tm_hour = bcd8_to_bin8(bcd->hour),
		.tm_mday = day,
		.tm_mon = month,
		.tm_year = y,
		.tm_wday = bcd->weekday,
		.tm_yday = d,
	};
	return __mktime32(&conv);
}

/**
 * set incorrect data to 0
 * 
 * @param bcd
 */
void bcdt_fix(bcd_time_t * restrict const bcd){
    const u8 second = bcd8_to_bin8(bcd->sec);
	const u8 minute = bcd8_to_bin8(bcd->min);
	const u8 hour = bcd8_to_bin8(bcd->hour);
	u8 day = bcd8_to_bin8(bcd->day);
	u8 month = bcd8_to_bin8(bcd->month);
	u8 year = bcd8_to_bin8(bcd->year);
    
    if(second < MAX_SEC){
        bcd->sec = bin8_to_bcd8(second);
    } else {
        bcd->sec = MIN_SEC_BCD;
    }
    if(minute < MAX_MIN){
        bcd->min = bin8_to_bcd8(minute);
    } else {
        bcd->min = MIN_MIN_BCD;
    }
    if(hour < MAX_HOUR){
        bcd->hour = bin8_to_bcd8(hour);
    } else {
        bcd->hour = MIN_HOUR_BCD;
    }
    if(MIN_DAY <= day && day < MAX_DAY){
        bcd->day = bin8_to_bcd8(day);
    } else {
        day = MIN_DAY;
        bcd->day = MIN_DAY_BCD;
    }
    if(MIN_MONTH <= month && month < MAX_MONTH){
        bcd->month = bin8_to_bcd8(month);
    } else {
        month = 1;
        bcd->month = 0x01;
    }
    if(MIN_YEAR <= year && year <= MAX_YEAR){
        bcd->year = bin8_to_bcd8(year);
    } else {
        year = MIN_YEAR;
        bcd->year = MIN_YEAR_BCD;
    }
    bcd->weekday = bcdt_dayofweek_by_values(BCD_YEAR_BASE + year, month, day);
}
