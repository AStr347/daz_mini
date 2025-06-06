#ifndef ALARMS_H
#define ALARMS_H
#include "ia_arch.h"
#include "bcd_time.h"

#define CLOCK_ALARM_COUNT		4
#define CLOCK_ALARM_FIRST		0
#define CLOCK_ALARM_SNOOZE		3	/* Nikita 26.02.20 */
/* MAX minute interval */
#define CLOCK_ALARM_SNOOZE_MAX		10
/**
 * defines state of clock alarm
 * @mem CLOCK_ALARM_DIS		- clock alarm neednt to proc;
 * @mem CLOCK_ALARM_WAIT	- clock alarm wait for minute
 * @mem CLOCK_ALARM_DONE	- clock alarm done today - was using for what?
 */
typedef enum  __packed {
	CLOCK_ALARM_DIS = 0,
	CLOCK_ALARM_WAIT = 1,
} clock_alarm_state_t;

/**
 * alarm daymask
 * every bit of this bitfield
 * defined day of week when alarm is active
 */
typedef union {
	struct {
		b8 sun0: 1;
		b8 mon: 1;
		b8 tue: 1;
		b8 wed: 1;
		b8 thu: 1;
		b8 fri: 1;
		b8 sat: 1;
		b8 sun: 1;
	};
	u8 raw;
} alarm_days_t;

/** 
 * clock alarm uses clock for emit alarm
 * messages when time is come
 * @mem hourmin	- minute in day	when alarm proc (hour*60 + min)
 * @mem days	- days of week when alarm proc
 * @mem state	- state of alarm clock
 */
typedef struct {
	union {
		struct {
			u8 min;
			u8 hour;
		};
		u16 hourmin;
	};
	alarm_days_t days;
	clock_alarm_state_t state;
    u16 snooze;
} clock_alarm_t;

/**
 * set alarm time, and days
 * @param num	- alarm clock number
 * @param bcd - time for set
 * @param days - days for alarm working
 * @return 
 */
extern b8 clock_alarm_set(const u8 num, const bcd_time_t* bcd, const alarm_days_t days);

/**
 * enable alarm clock
 * @param num	- alarm clock number
 */
extern void clock_alarm_en(const u8 num);

/**
 * disable alarm clock
 * @param num	- alarm clock number
 */
extern void clock_alarm_dis(const u8 num);

/**
 * return current alarm parametrs
 * @param	- num set current alarm number  
 * @return	- current alarm parametrs
 */
extern clock_alarm_t* clock_alarm_data_get(const u8 num);


/**
 * set alarm snooze
 * interval minutes if 0 - disable snooze!
 */
extern b8 clock_alarm_snooze_set(const u8 num, const u16 interval);


/**
 * create time of alarm with snooze
 * @param num
 * @return u8_0 - hour; u8_1 - min
 */
extern u8_u8 clcok_alarm_get_time(const u8 num);

/**
 * return true if need proc alarm
 */
extern b8_u8 clock_alarm_ctl(void);

#endif
