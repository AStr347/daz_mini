#define NET_MENU_ST
/**
 * Main menu states for table receiver
 *
 */

#define STATE_NAME		net_menu_st

#include "app.h"

typedef enum{
	NM_CONNECT,
    NM_EDIT,
	NM_COUNT,
} cur_net_menus;

typedef struct {
    u8 index;
	u8 timeout;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

static const txt_desktop_t mainmenu_captions[] = {
    [NM_CONNECT] = TXT_NM_CONNECT,
    [NM_EDIT] = TXT_NM_EDIT,
};

static const ine_state_t * const jumps[] = {
    [NM_CONNECT] = &connect_st,
    [NM_EDIT] = &devlist_st,
};

/** helper functions **/
GS_SCREEN(net_menu, NULL, NULL)

static
void update(void)
{
    SDATA->timeout = TIMEOUT;

    const u8 * const line0 = TXT(TXT_MM_ALARM);
    net_menu_screen.lines[0] = line0;
    net_menu_screen.lens[0] = 8;

    const u8 * const line1 = TXT(mainmenu_captions[SDATA->index]);
    net_menu_screen.lines[1] = line1;
    net_menu_screen.lens[1] = strnlen(line1, 8);

	G_DRAWSCREEN(net_menu_screen);
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
    if(NM_CONNECT == SDATA->index){
        SDATA->index = NM_EDIT;
    } else {
        SDATA->index--;
    }
	update();
}

ON_RIGHT_P()
{
    if(NM_EDIT == SDATA->index){
        SDATA->index = NM_CONNECT;
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


