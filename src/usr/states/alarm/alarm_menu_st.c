#define ALARM_MENU_ST
/**
 * Main menu states for table receiver
 *
 */

#define STATE_NAME		alarm_menu_st

#include "app.h"

typedef enum{
	AM_ENDIS,
    AM_TIME,
    AM_MELODY,
    AM_SNOOZE,
	AM_COUNT,
} cur_net_menus;

typedef struct {
    u8 index;
	u8 timeout;
    clock_alarm_t * cal;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

static const txt_desktop_t mainmenu_captions[] = {
    [AM_ENDIS] = TXT_AM_DIS,
    [AM_TIME] = TXT_AM_TIME,
    [AM_MELODY] = TXT_AM_MELODY,
    [AM_SNOOZE] = TXT_AM_SNOOZE,
};

static const ine_state_t * const jumps[] = {
    [AM_ENDIS] = NULL,
    [AM_TIME] = &alarm_time_st,
    [AM_MELODY] = &alarm_melody_st,
    [AM_SNOOZE] = &alarm_snooze_st,
};

/** helper functions **/
GS_SCREEN(alarm_menu, NULL, NULL)

static
void update(void)
{
    SDATA->timeout = TIMEOUT;

    const u8 * const line0 = TXT(TXT_MM_ALARM);
    alarm_menu_screen.lines[0] = line0;
    alarm_menu_screen.lens[0] = 8;

    const u8 * line1 = NULL;
    if(AM_ENDIS == SDATA->index){
        const b8 en = (CLOCK_ALARM_DIS < SDATA->cal->state);
        line1 = TXT(mainmenu_captions[SDATA->index] + en);
    } else {
        line1 = TXT(mainmenu_captions[SDATA->index]);
    }
    
    alarm_menu_screen.lines[1] = line1;
    alarm_menu_screen.lens[1] = 8;

	G_DRAWSCREEN(alarm_menu_screen);
}

PROLOG()
{
    SDATA->index = 0;
    SDATA->timeout = TIMEOUT;
    SDATA->cal = clock_alarm_data_get(CLOCK_ALARM_FIRST);
	update();
}

EPILOG()
{
	graphic_clear(&graphics);
}

ON_SEC()
{
	if(SDATA->timeout){
		--SDATA->timeout;
	}else{
		STATE_JMP(&main_menu_st);
	}
}

ON_ESCAPE_P()
{
	STATE_JMP(&main_menu_st);
}

ON_ENTER_P()
{
    if(AM_ENDIS == SDATA->index){
        const b8 al = (CLOCK_ALARM_DIS != SDATA->cal->state);
        if(true == al){
            clock_alarm_dis(CLOCK_ALARM_FIRST);
        }else{
            clock_alarm_en(CLOCK_ALARM_FIRST);
        }
        update();
    } else {
	    STATE_JMP(jumps[SDATA->index]);
    }
}

ON_LEFT_P()
{
    if(AM_ENDIS == SDATA->index){
        SDATA->index = AM_SNOOZE;
    } else {
        SDATA->index--;
    }
	update();
}

ON_RIGHT_P()
{
    if(AM_SNOOZE == SDATA->index){
        SDATA->index = AM_ENDIS;
    } else {
        SDATA->index++;
    }
	update();
}

INE_STATE(STATE_NAME, 7)
{
	.count = 7,
    .block[0] = PROLOG_EVENT,
    .block[1] = EPILOG_EVENT,
    .block[2] = SEC_EVENT,
    .block[3] = ESCAPE_P_EVENT,
    .block[4] = ENTER_P_EVENT,
    .block[5] = LEFT_P_EVENT,
    .block[6] = RIGHT_P_EVENT,
};


