#include "app.h"
#include "states.h"

const u8 long_to_config_translator[LONG_COUNT] = {
    [0] = 5,
    [1] = 10,
    [2] = 15,
    [3] = 20,
    [4] = 25,
    [5] = 30,
    [6] = 35,
    [7] = 40,
    [8] = 45,
    [9] = 50,
    [10] = 55,
    [11] = 60,
};

void do_increment(u8 * const digit,
                  const t_change_type increment,
                  const u8 minimum,
                  const u8 maximum)
{
	s8 d = *digit;
	if(increment == DECREASE){
		d--;
	}else if(increment == INCREASE){
		d++;
	}
	if(d < minimum){
		d = maximum;
	}else if(d > maximum || d < minimum){
		d = minimum;
	}
	*digit = d;
}

u8 * fill_time(u8 * dst,
               const bcd_time_t * const dtime)
{
    *dst++ = '0' + ((dtime->hour >> 4) & 0xf);
    *dst++ = '0' + ((dtime->hour >> 0) & 0xf);
    *dst++ = ':';
    *dst++ = '0' + ((dtime->min >> 4) & 0xf);
    *dst++ = '0' + ((dtime->min >> 0) & 0xf);
    
    return dst;
}

u8 * fill_date(u8 * dst,
               const bcd_time_t * const dtime)
{
    const u8 * src = TXT(TXT_SUN_S + dtime->weekday);
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = ' ';
    *dst++ = '0' + ((dtime->day >> 4) & 0xf);
    *dst++ = '0' + ((dtime->day >> 0) & 0xf);
    *dst++ = '.';
    *dst++ = '0' + ((dtime->month >> 4) & 0xf);
    *dst++ = '0' + ((dtime->month >> 0) & 0xf);

    return dst;
}
