#define CLOCK_ALARM_MODULE
#include "ia_arch.h"
#include "alarms_internal.h"
#include "alarms.h"
#include "rtc_drv.h"

/**
 * set alarm time, and days
 * @param num	- alarm clock number
 * @param bcd - time for set
 * @param days - days for alarm working
 * @return 
 */
b8 clock_alarm_set(const u8 num, const bcd_time_t* bcd, const alarm_days_t days){
    if(num < CLOCK_ALARM_COUNT){
        clock_alarm_t* ca = calarm + num;
        ca->hour = bcd->hour;
        ca->min = bcd->min;
        ca->days = days;
        ca->state = CLOCK_ALARM_WAIT;
        ca->snooze = 0;
        return true;
    }
    return false;
}

/**
 * enable alarm clock
 * @param num	- alarm clock number
 */
void clock_alarm_en(const u8 num){
    if(num < CLOCK_ALARM_COUNT){
        clock_alarm_t* ca = calarm + num;
        ca->snooze = 0;
        ca->state = CLOCK_ALARM_WAIT;
    }
}

/**
 * disable alarm clock
 * @param num	- alarm clock number
 */
void clock_alarm_dis(const u8 num){
    if(num < CLOCK_ALARM_COUNT){
        clock_alarm_t* ca = calarm + num;
        ca->snooze = 0;
        ca->state = CLOCK_ALARM_DIS;
    }
}

/**
 * return current alarm parametrs
 * @param	- num set current alarm number  
 * @return	- current alarm parametrs
 */
clock_alarm_t* clock_alarm_data_get(const u8 num){
    clock_alarm_t* result = NULL;
    if(num < CLOCK_ALARM_COUNT){
        result = calarm + num;
    }
    return result;
}


/**
 * set alarm snooze
 * interval minutes if 0 - disable snooze!
 */
b8 clock_alarm_snooze_set(const u8 num, const u16 interval){
    b8 result = false;
    if(num < CLOCK_ALARM_COUNT){
        clock_alarm_t* ca = calarm + num;
        ca->snooze = interval;
        result = true;
    }
    return result;
}

/**
 * create time of alarm with snooze
 * @param num
 * @return u8_0 - hour; u8_1 - min
 */
u8_u8 clcok_alarm_get_time(const u8 num){
    u8_u8 result = {.u8_0 = 0xff, .u8_1 = 0xff};
    if(num < CLOCK_ALARM_COUNT){
        clock_alarm_t* ca = calarm + num;
        const u16 snooze = ca->snooze;
        if(0 != snooze){
            u16 hour = (snooze / 60);
            u16 min = (snooze % 60);
            hour += bcd8_to_bin8(ca->hour);
            min += bcd8_to_bin8(ca->min);
            const u8 leakhour = min / 60;
            hour += leakhour;
            min %= 60;
            hour %= 24;
            result.u8_0 = bin8_to_bcd8(hour);
            result.u8_1 = bin8_to_bcd8(min);
        } else {
            result.u8_0 = ca->hour;
            result.u8_1 = ca->min;
        }
    }
    return result;
}

/**
 * return true if need proc alarm
 */
b8_u8 clock_alarm_ctl(void){
    b8_u8 result = {.b8 = false, .u8 = 0xff};
    for (u8 num = 0; num < CLOCK_ALARM_COUNT; num++){
        const clock_alarm_t* ca = calarm + num;
        if(CLOCK_ALARM_WAIT == ca->state){
            const u8_u8 hourmin = clcok_alarm_get_time(num);
            const bcd_time_t dtime = rtc_bcd_get();
            const b8 proc = (hourmin.u8_0 == dtime.hour) &&
                    		(hourmin.u8_1 == dtime.min);

            if(true == proc){
                result.b8 = true;
                result.u8 = num;
                break;
            }
        }
    }
    return result;
}
