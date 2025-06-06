#define MAIN_MENU_ST
/**
 * Main menu states for table receiver
 *
 */

#define STATE_NAME		main_menu_st

#include "app.h"
#define MAINMENU_TIMEOUT	(TIMEOUT)

static u8 menu_index = 0;

typedef enum{
	MAIN_JOURNAL = 0,
	MAIN_ALARM,
	MAIN_TIME,
	MAIN_DATE,
	MAIN_VOLUME,
	MAIN_NIGHT_COLOR,
	MAIN_LONG_SIGNAL,
    MAIN_MODE,
	MAIN_BUTTONS,
	MAIN_COUNT,
} cur_net_menus;

typedef struct {
    u8 min;
    u8 max;
	u8 timeout;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

static const txt_desktop_t mainmenu_captions[] = {
    [MAIN_JOURNAL] = TXT_MM_JOURNAL,
	[MAIN_ALARM] = TXT_MM_ALARM,
	[MAIN_TIME] = TXT_MM_TIME,
	[MAIN_DATE] = TXT_MM_DATE,
	[MAIN_VOLUME] = TXT_MM_VOL,
	[MAIN_NIGHT_COLOR] = TXT_MM_NIGHT_COLOR,
	[MAIN_LONG_SIGNAL] = TXT_MM_LONG_SIGNAL,
	[MAIN_MODE] = TXT_MM_MODE,
	[MAIN_BUTTONS] = TXT_MM_NET,
};

static const ine_state_t * const jumps[] = {
    [MAIN_JOURNAL] = &journal_st,
    [MAIN_ALARM] = &alarm_menu_st,
    [MAIN_TIME] = &edit_time_st,
    [MAIN_DATE] = &edit_date_st,
    [MAIN_VOLUME] = &edit_vol_st,
    [MAIN_NIGHT_COLOR] = &edit_night_color_st,
    [MAIN_LONG_SIGNAL] = &edit_long_signal_st,
    [MAIN_MODE] = &edit_mode_st,
    [MAIN_BUTTONS] = &net_menu_st,
};

/** helper functions **/
GS_SCREEN(main_menu, NULL, NULL)

static
void update(void)
{
	SDATA->timeout = TIMEOUT;

    const u8 * const line1 = TXT(mainmenu_captions[menu_index]);
    main_menu_screen.lines[1] = line1;
    main_menu_screen.lens[1] = strnlen(line1, 8);

	G_DRAWLINE(main_menu_screen, 1);
}

PROLOG()
{
    SDATA->max = MAIN_COUNT - 1;
    SDATA->min = MAIN_JOURNAL;
    SDATA->timeout = MAINMENU_TIMEOUT;
    /* need journal */
    const b8 njournal = (0 < journal.list.cnt);
    /* need move index */
    const b8 nmove = (MAIN_JOURNAL == menu_index);
    if(true == nmove){
        if(false == njournal){
            menu_index = MAIN_ALARM;
        }
    }
    /* cut journal */
    if(false == njournal){
        SDATA->min = MAIN_ALARM;
    }
    
    const u8 * const line0 = TXT(TXT_MM);
    main_menu_screen.lines[0] = line0;
    main_menu_screen.lens[0] = strnlen(line0, 8);
    G_DRAWLINE(main_menu_screen, 0);
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
		menu_index = MAIN_JOURNAL;
		STATE_JMP(&standby_st);
	}
}

ON_ESCAPE_P()
{
	menu_index = MAIN_JOURNAL;
	STATE_JMP(&standby_st);
}

ON_ENTER_P()
{
	STATE_JMP(jumps[menu_index]);
}

ON_LEFT_P()
{
    if(menu_index > SDATA->min){
        --menu_index;
    } else {
        menu_index = SDATA->max;
    }
	update();
}

ON_RIGHT_P()
{
    if(menu_index < SDATA->max){
        ++menu_index;
    } else {
        menu_index = SDATA->min;
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


