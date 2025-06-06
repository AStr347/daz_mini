#ifndef STATES_H
#define STATES_H
#include "inevent.h"
#include "inevent_state.h"

#define BASE_LONG_HOLD      (10)
#define STANDBY_LONG_PRESS  (3)
#define SUPER_LONG_HOLD     (10)
#define MIN_LONG_HOLD       (3)

typedef enum {
	INCREASE,
	DECREASE,
	NONE,
} t_change_type;

#define LONG_COUNT 12
#define SNOOZE_COUNT 10

extern
const u8 long_to_config_translator[LONG_COUNT];

extern
const color_t color_list[LED_COUNT];

extern
void do_increment(u8 * const digit,
                  const t_change_type increment,
                  const u8 minimum,
                  const u8 maximum);

extern
u8 * fill_time(u8 * dst,
               const bcd_time_t * const dtime);

extern
u8 * fill_date(u8 * dst,
               const bcd_time_t * const dtime);

#ifndef BOOT_ST
extern const ine_state_t boot_st;
#endif

#ifndef POWER_OFF_ST
extern const ine_state_t power_off_st;
#endif

#ifndef STANDBY_ST
extern const ine_state_t standby_st;
#endif

#ifndef MAIN_MENU_ST
extern const ine_state_t main_menu_st;
#endif

#ifndef JOURNAL_ST
extern const ine_state_t journal_st;
#endif

#ifndef EDIT_TIME_ST
extern const ine_state_t edit_time_st;
#endif

#ifndef EDIT_DATE_ST
extern const ine_state_t edit_date_st;
#endif

#ifndef EDIT_VOL_ST
extern const ine_state_t edit_vol_st;
#endif

#ifndef EDIT_NIGHT_COLOR_ST
extern const ine_state_t edit_night_color_st;
#endif

#ifndef EDIT_LONG_SIGNAL_ST
extern const ine_state_t edit_long_signal_st;
#endif

#ifndef EDIT_MODE_ST
extern const ine_state_t edit_mode_st;
#endif

#ifndef ALARM_MENU_ST
extern const ine_state_t alarm_menu_st;
#endif

#ifndef ALARM_TIME_ST
extern const ine_state_t alarm_time_st;
#endif

#ifndef ALARM_MELODY_ST
extern const ine_state_t alarm_melody_st;
#endif

#ifndef ALARM_SNOOZE_ST
extern const ine_state_t alarm_snooze_st;
#endif

#ifndef NET_MENU_ST
extern const ine_state_t net_menu_st;
#endif

#ifndef NET_CONNECT_ST
extern const ine_state_t connect_st;
#endif

#ifndef NET_DEVLIST_ST
extern const ine_state_t devlist_st;
#endif

#ifndef NET_EDIT_DEV_ST
extern const ine_state_t net_edit_dev_st;
#endif

#ifndef NET_DEV_EDIT_NAME_ST
extern const ine_state_t dedit_name_st;
#endif

#ifndef NET_DEV_EDIT_MELODY_ST
extern const ine_state_t dedit_melody_st;
#endif

#ifndef NET_DEV_EDIT_COLOR_ST
extern const ine_state_t dedit_color_st;
#endif

#ifndef NET_DEV_EDIT_VIBRO_ST
extern const ine_state_t dedit_vibro_st;
#endif


#endif//STATES_H
