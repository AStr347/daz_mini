#define NET_EDIT_DEV_ST
/**
 * Main menu states for table receiver
 *
 */

#define STATE_NAME		net_edit_dev_st

#include "app.h"

typedef enum{
	NED_NAME0,
    NED_NAME1,
    NED_MELODY,
    NED_COLOR,
    NED_VIBRO,
	NED_COUNT,
} cur_net_menus;

typedef struct {
    u8 index;
	u8 timeout;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

static const txt_desktop_t mainmenu_captions[] = {
    [NED_NAME0] = TXT_NMED_NAME0,
    [NED_NAME1] = TXT_NMED_NAME1,
    [NED_MELODY] = TXT_NMED_MELODY,
    [NED_COLOR] = TXT_NMED_COLOR,
    [NED_VIBRO] = TXT_NMED_VIBRO,
};

static const ine_state_t * const jumps[] = {
    [NED_NAME0] = &dedit_name_st,
    [NED_NAME1] = &dedit_name_st,
    [NED_MELODY] = &dedit_melody_st,
    [NED_COLOR] = &dedit_color_st,
    [NED_VIBRO] = &dedit_vibro_st,
};

/** helper functions **/
GS_SCREEN(edit_dev, NULL, NULL)

static
void update(void)
{
    SDATA->timeout = TIMEOUT;

    const u8 * const line0 = TXT(TXT_MM_ALARM);
    edit_dev_screen.lines[0] = line0;
    edit_dev_screen.lens[0] = 8;

    const u8 * const line1 = TXT(mainmenu_captions[SDATA->index]);
    edit_dev_screen.lines[1] = line1;
    edit_dev_screen.lens[1] = strnlen(line1, 8);

	G_DRAWSCREEN(edit_dev_screen);
}

PROLOG()
{
    SDATA->index = 0;
    SDATA->timeout = TIMEOUT;
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
    STATE_JMP(jumps[SDATA->index]);
}

ON_LEFT_P()
{
    if(NED_NAME0 == SDATA->index){
        SDATA->index = NED_VIBRO;
    } else {
        SDATA->index--;
    }
	update();
}

ON_RIGHT_P()
{
    if(NED_VIBRO == SDATA->index){
        SDATA->index = NED_NAME0;
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


