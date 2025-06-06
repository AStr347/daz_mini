#define STANDBY_ST
#define STATE_NAME 		standby_st
#include "app.h"

typedef struct {
	bcd_time_t last_time;
	char alarm_char;
	char battery_char;
} sdata_t;
SDATA_DEF;

u8 standby_line0[GRAPHIC_BE_COLUMN_CNT + 1] = "WW DD.MM";
u8 standby_line1[GRAPHIC_BE_COLUMN_CNT + 1] = "B HH:MMA";
GS_SCREEN(standby, standby_line0, standby_line1)

static
char battery_icon(void){
	static const
	char charge_p2char[UPOWERL_LEVEL_COUNT] = {
		[UPOWERL_NEED_OFF_P] = '\x01',
		[UPOWERL_LOW_P] = '\x02',
		[UPOWERL_NORMAL_P] = '\x03',
		[UPOWERL_PREFULL_P] = '\x04',
		[UPOWERL_FULL_P] = '\x04',
	};

	const u8 charge = upower_charge(&upower0);
	u8 charge_p = UPOWERL_NEED_OFF_P;
	for(u8 i = 0; i < UPOWERL_LEVEL_COUNT; i++){
		const u8 level = upower_pos2level[i];
		if(level == charge){
			charge_p = i;
		}
	}
	
	return charge_p2char[charge_p];
}

static
void update(void){
	b8 need_expose = false;
	const char new_battery = battery_icon();
	const bcd_time_t dtime = rtc_bcd_get();
    {
		/* date line */
		if(dtime.raw_date != SDATA->last_time.raw_date){
			need_expose = true;

			u8 * dst = standby_line0;
			dst = fill_date(dst, &dtime);
			*dst++ = '\0';
			G_DRAWLINE(standby_screen, 0);
		}
    }

    {
		/* time line */
		const b8 time_update = dtime.raw_time != SDATA->last_time.raw_time;
		const b8 battery_updated = SDATA->battery_char != new_battery;
		if(time_update || battery_updated){
			need_expose = true;

			u8 * dst = standby_line1;
			*dst++ = new_battery;
			*dst++ = ' ';
			dst = fill_time(dst, &dtime);
			*dst++ = SDATA->alarm_char;
			*dst++ = '\0';
			G_DRAWLINE(standby_screen, 1);
		}
	}

    if(need_expose){
    	graphic_expose(&graphics);
    }
    SDATA->last_time = dtime;
	SDATA->battery_char = new_battery;
}

PROLOG()
{
	graphic_clear(&graphics);

	SDATA->last_time = (bcd_time_t){0};

	const clock_alarm_t * const restrict cal = clock_alarm_data_get(0);
    const b8 alarm_en = (CLOCK_ALARM_DIS < cal->state);
	SDATA->alarm_char = alarm_en? '\x09' : ' ';
	SDATA->battery_char = ' ';

	graphic_loadextr(&graphics, '\x01', BCB0802_EXTS_BAT_EMPTY);
	graphic_loadextr(&graphics, '\x02', BCB0802_EXTS_BAT_LOW);
	graphic_loadextr(&graphics, '\x03', BCB0802_EXTS_BAT_MID);
	graphic_loadextr(&graphics, '\x04', BCB0802_EXTS_BAT_FULL);

    update();
    G_DRAWSCREEN(standby_screen);
	net0_normal();
	//buzzer_play(BUZ_INTERCOM, 2);
}

EPILOG()
{
	graphic_clear(&graphics);
	net0_off();
}

ON_MIN()
{
	update();
	//buzzer_play(BUZ_INTERCOM, 2);
}

ON_ENTER_P()
{
	STATE_JMP(&main_menu_st);
}

ON_ESCAPE_H(const u8 time)
{
	if(40 == time){
		STATE_JMP(&power_off_st);
	}
}

ON_SEC()
{
	
}

INE_STATE(STATE_NAME, 6) {
	.count = 6,
	.block[0] = PROLOG_EVENT,
	.block[1] = EPILOG_EVENT,
	.block[2] = SEC_EVENT,
	.block[3] = MIN_EVENT,
	.block[4] = ENTER_P_EVENT,
	.block[5] = ESCAPE_H_EVENT,
};
