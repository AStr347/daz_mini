#define EDIT_DATE_ST
/*
 * edit system time state
 */

#define STATE_NAME		edit_date_st

#include "app.h"

#define SEL_COLOR		BLACK
#define USEL_COLOR		WHITE
#define BLINK_DELAY		2

typedef enum __packed {
	ET_FIRST = 0,
	ET_DAY = 0,
	ET_MONTH,
	ET_YEAR,
	ET_LAST = ET_YEAR,
} edit_part_t;

typedef struct {
	u8 timeout;
	u8 blink_delay;
    b8 blink;
    b8 pressed;
    edit_part_t index;

    bcd_time_t curr_time;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

u8 date_str[8] = "DD.MM.YY";
GS_SCREEN(edit_date, NULL, date_str);

static
void changeDateTime(const t_change_type increment)
{
	u8 d = bcd8_to_bin8(SDATA->curr_time.day);
	u8 m = bcd8_to_bin8(SDATA->curr_time.month);
	u8 y = bcd8_to_bin8(SDATA->curr_time.year);
    u8 max_day = bcdt_month_daycount_by_values(BCDDY2TM(y), m);

	switch(SDATA->index){
		case ET_DAY:{
			do_increment(&d, increment, 1, max_day);
        } break;
		case ET_MONTH:{
			do_increment(&m, increment, 1, 12);
        } break;
		case ET_YEAR:{
			do_increment(&y, increment, MIN_YEAR, MAX_YEAR);
        } break;
	}
    max_day = bcdt_month_daycount_by_values(BCDDY2TM(y), m);
    
	if(d > max_day) { d = max_day; }
    
	SDATA->curr_time.weekday = bcdt_dayofweek_by_values(BCD_YEAR_BASE + y, m, d);
	SDATA->curr_time.day = bin8_to_bcd8(d);
	SDATA->curr_time.month = bin8_to_bcd8(m);
	SDATA->curr_time.year = bin8_to_bcd8(y);
}

/**
 * on enter standby state
 */
static
void set_current_datetime(void)
{
    const u8 day = bcd8_to_bin8(SDATA->curr_time.day);
    const b8 wrong_day = (0 == day) || (31 < day);
	if(true == wrong_day){
		SDATA->curr_time.day = 0x01;
	}
    const u8 month = bcd8_to_bin8(SDATA->curr_time.month);
    const b8 wrong_month = (0 == month) || (12 < month);
	if(true == wrong_month){
		SDATA->curr_time.month = 0x01;
	}
    const u8 year = bcd8_to_bin8(SDATA->curr_time.year);
    const b8 wrong_year = (MIN_YEAR > year) || (MAX_YEAR < year);
	if(true == wrong_year){
        SDATA->curr_time.year = MIN_YEAR_BCD;
	}
}

/**
 * 
 */
static
void update(void)
{
	SDATA->timeout = TIMEOUT;
    const bcd_time_t * const dtime = &SDATA->curr_time;
    u8 * dst = date_str;
    *dst++ = '0' + ((dtime->day >> 4) & 0xf);
    *dst++ = '0' + ((dtime->day >> 0) & 0xf);
    *dst++ = '.';
    *dst++ = '0' + ((dtime->month >> 4) & 0xf);
    *dst++ = '0' + ((dtime->month >> 0) & 0xf);
    *dst++ = '.';
    *dst++ = '0' + ((dtime->year >> 4) & 0xf);
    *dst++ = '0' + ((dtime->year >> 0) & 0xf);

    const u8 * const line1 = date_str;
    edit_date_screen.lines[1] = line1;
    edit_date_screen.lens[1] = 8;

    G_DRAWLINE(edit_date_screen, 1);
}

/*
 * state prolog
 */
PROLOG()
{
    SDATA->timeout = TIMEOUT;
	SDATA->blink_delay = 0;
    SDATA->pressed = false;
    SDATA->index = ET_FIRST;
    SDATA->curr_time = rtc_bcd_get();
    SDATA->blink = true;
    
    set_current_datetime();
	
    const u8 * const line0 = TXT(TXT_MM_DATE);
    edit_date_screen.lines[0] = line0;
    edit_date_screen.lens[0] = 8;
    graphic_clear(&graphics);
    G_DRAWLINE(edit_date_screen, 0);
	update();
}

EPILOG()
{
    const bcd_time_t curr_time = rtc_bcd_get();
	SDATA->curr_time.hour = curr_time.hour;
	SDATA->curr_time.min = curr_time.min;
    SDATA->curr_time.sec = curr_time.sec;
	rtc_bcd_set(&(SDATA->curr_time));
}
    
/*
 * exit state
 */
ON_ESCAPE_P() { STATE_JMP(&main_menu_st); }

ON_RIGHT_H(const u8 time){
    if(BASE_LONG_HOLD < time){
        changeDateTime(INCREASE);
        SDATA->pressed = true;
        update();
    }
}

ON_LEFT_H(const u8 time){
    if(BASE_LONG_HOLD < time){
        changeDateTime(DECREASE);
        SDATA->pressed = true;
        update();
    }
}

/*
 * event half sec
 */
ON_HSEC()
{
	if(false == SDATA->pressed){
		if(0 < SDATA->blink_delay){
			--SDATA->blink_delay;
		}else{
            SDATA->blink = !SDATA->blink;
            SDATA->blink_delay = BLINK_DELAY;
            switch (SDATA->index){
                case ET_DAY: {
                    if(SDATA->blink){
                        graphic_set_char(&graphics, ' ', 0, 1);
                        graphic_set_char(&graphics, ' ', 1, 1);
                    } else {
                        graphic_set_char(&graphics, date_str[0], 0, 1);
                        graphic_set_char(&graphics, date_str[1], 1, 1);
                    }
                } break;
                case ET_MONTH: {
                    if(SDATA->blink){
                        graphic_set_char(&graphics, ' ', 3, 1);
                        graphic_set_char(&graphics, ' ', 4, 1);
                    } else {
                        graphic_set_char(&graphics, date_str[3], 3, 1);
                        graphic_set_char(&graphics, date_str[4], 4, 1);
                    }
                } break;
                case ET_YEAR: {
                    if(SDATA->blink){
                        graphic_set_char(&graphics, ' ', 6, 1);
                        graphic_set_char(&graphics, ' ', 7, 1);
                    } else {
                        graphic_set_char(&graphics, date_str[6], 6, 1);
                        graphic_set_char(&graphics, date_str[7], 7, 1);
                    }
                } break;
            }
		}
	}

	if(likely(SDATA->timeout)){
		--SDATA->timeout;
	}else{
		STATE_JMP(&standby_st);
	}
}

ON_ENTER_P()
{
	if(SDATA->index == ET_LAST){
		STATE_JMP(&main_menu_st);
	}else{
		do_increment((u8*)&SDATA->index, INCREASE, ET_FIRST, ET_LAST);
        G_DRAWSCREEN(edit_date_screen);
	}
}

/*
 * key up press
 */
ON_LEFT_P()
{
	SDATA->blink_delay = BLINK_DELAY;
	changeDateTime(DECREASE);
	update();
}

/*
 * key down press
 */
ON_RIGHT_P()
{
	SDATA->blink_delay = BLINK_DELAY;
	changeDateTime(INCREASE);
	update();
}

ON_LEFT_R(){ SDATA->pressed = false; }
ON_RIGHT_R() { SDATA->pressed = false; }

INE_STATE(STATE_NAME, 11)
{
	.count = 11,
    .block[0] = PROLOG_EVENT,
    .block[1] = EPILOG_EVENT,
    .block[2] = HSEC_EVENT,
    .block[3] = ESCAPE_P_EVENT,
    .block[4] = ENTER_P_EVENT,
    .block[5] = LEFT_P_EVENT,
    .block[6] = LEFT_H_EVENT,
    .block[7] = RIGHT_P_EVENT,
    .block[8] = RIGHT_H_EVENT,
    .block[9] = LEFT_R_EVENT,
    .block[10] = RIGHT_R_EVENT,
};

