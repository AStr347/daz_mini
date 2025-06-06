#define EDIT_TIME_ST
/*
 * edit system time state
 */

#define STATE_NAME		edit_time_st


#include "app.h"

#define SEL_COLOR		BLACK
#define USEL_COLOR		WHITE

#define BLINK_DELAY		2

typedef enum{
	ET_HOUR,
	ET_MIN,
	ET_COUNT,
} edit_part_t;

typedef struct {
	u8 timeout;
	u8 index;
	u8 blink_delay;
    b8 blink;
    b8 pressed;

	struct{
		u8 min;
		u8 hour;
	} params;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

static
u8 time_str[8] = "HH:MM";
GS_SCREEN(edit_time, NULL, time_str);

static
void update(void)
{
	u8 * dst = time_str;
    *dst++ = '0' + (SDATA->params.hour / 10);
    *dst++ = '0' + (SDATA->params.hour % 10);
    *dst++ = ':';
    *dst++ = '0' + (SDATA->params.min / 10);
    *dst++ = '0' + (SDATA->params.min % 10);

    const u8 * const line1 = time_str;
    edit_time_screen.lines[1] = line1;
    edit_time_screen.lens[1] = 5;

    G_DRAWLINE(edit_time_screen, 1);
    graphic_expose(&graphics);
}

static
void changeDateTime(const t_change_type increment)
{
    if(ET_HOUR == SDATA->index){
        do_increment(&(SDATA->params.hour), increment, 0, 23);
    } else if (ET_MIN == SDATA->index) {
        do_increment(&(SDATA->params.min), increment, 0, 59);
    } else {
        STATE_JMP(&main_menu_st);
    }
    update();
	SDATA->timeout = TIMEOUT;
}

/*
 * state prolog
 */
PROLOG()
{
	SDATA->blink_delay = 0;
	SDATA->index = ET_HOUR;
    SDATA->pressed = false;
	SDATA->timeout = TIMEOUT;
    SDATA->blink = false;

    const bcd_time_t curr_time = rtc_bcd_get();

	SDATA->params.hour = bcd8_to_bin8(curr_time.hour);
	SDATA->params.min = bcd8_to_bin8(curr_time.min);

	SDATA->index = ET_HOUR;

    const u8 * const line0 = TXT(TXT_MM_TIME);
    edit_time_screen.lines[0] = line0;
    edit_time_screen.lens[0] = strnlen(line0, 8);
    
    graphic_clear(&graphics);
    G_DRAWLINE(edit_time_screen, 0);
	update();
}

/*
 * state epilog
 */
EPILOG()
{
    bcd_time_t curr_time = rtc_bcd_get();
    curr_time.hour = bin8_to_bcd8(SDATA->params.hour);
    curr_time.min = bin8_to_bcd8(SDATA->params.min);
    curr_time.sec = 0;
	rtc_bcd_set(&curr_time);
}

/*
 * exit state
 */
ON_ESCAPE_P()
{
	STATE_JMP(&main_menu_st);
}

/*
 * event one sec
 */
ON_HSEC()
{
    /* proceed timeout */
    if(likely(SDATA->timeout)){
        --SDATA->timeout;
    }else{
        STATE_JMP(&standby_st);
        return;
    }

	if(false == SDATA->pressed){
		if(SDATA->blink_delay){
			--SDATA->blink_delay;
		}else{
            SDATA->blink = !SDATA->blink;
            SDATA->blink_delay = BLINK_DELAY;
			switch(SDATA->index){
				case ET_HOUR: {
					if(SDATA->blink){
                        graphic_set_char(&graphics, ' ', 0, 1);
                        graphic_set_char(&graphics, ' ', 1, 1);
                    } else {
                        graphic_set_char(&graphics, time_str[0], 0, 1);
                        graphic_set_char(&graphics, time_str[1], 1, 1);
                    }
				} break;
				case ET_MIN: {
					if(SDATA->blink){
                        graphic_set_char(&graphics, ' ', 3, 1);
                        graphic_set_char(&graphics, ' ', 4, 1);
                    } else {
                        graphic_set_char(&graphics, time_str[3], 3, 1);
                        graphic_set_char(&graphics, time_str[4], 4, 1);
                    }
				} break;
			}
		}
	}
}

ON_ENTER_P()
{
	if(SDATA->index == ET_MIN){
		STATE_JMP(&main_menu_st);
		return;
	}else{
        SDATA->index = ET_MIN;
        G_DRAWSCREEN(edit_time_screen);
		update();
	}
}

/*
 * key up press
 */
ON_LEFT_P()
{
	SDATA->blink_delay = BLINK_DELAY;
	changeDateTime(DECREASE);
}

ON_LEFT_H(const u8 time){
    if(BASE_LONG_HOLD < time){
        SDATA->pressed = true;
        changeDateTime(DECREASE);
    }
}

/*
 * key up relise
 */
ON_LEFT_R()
{
    SDATA->pressed = false;
}

/*
 * key down press
 */
ON_RIGHT_P()
{
	SDATA->blink_delay = BLINK_DELAY;
	changeDateTime(INCREASE);
}

ON_RIGHT_H(const u8 time){
    if(BASE_LONG_HOLD < time){
        SDATA->pressed = true;
        changeDateTime(INCREASE);
    }
}

/*
 * key right release
 */
ON_RIGHT_R()
{
    SDATA->pressed = false;
}

INE_STATE(STATE_NAME, 11)
{
	.count = 11,
    .block[0] = PROLOG_EVENT,
    .block[1] = EPILOG_EVENT,
    .block[2] = HSEC_EVENT,
    .block[3] = ESCAPE_P_EVENT,
    .block[4] = ENTER_P_EVENT,
    .block[5] = LEFT_P_EVENT,
    .block[6] = LEFT_R_EVENT,
    .block[7] = RIGHT_P_EVENT,
    .block[8] = RIGHT_R_EVENT,
    .block[9] = RIGHT_H_EVENT,
    .block[10] = LEFT_H_EVENT,
};



