#define EDIT_NIGHT_COLOR_ST
/*
 * edit night lamp color, red is cool for example
 */
#define STATE_NAME		edit_night_color_st

#include "app.h"

/*
 * current_edit_state=edit_state_brightness; 
 */
typedef struct {
	u8 timeout;
	u8 index;
} state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

GS_SCREEN(edit_color, NULL, NULL)

static
void update(void)
{
	SDATA->timeout = TIMEOUT;

	led_addr_set(color_list[SDATA->index]);
	const u8 * const line0 = TXT(TXT_MM_NIGHT_COLOR);
    edit_color_screen.lines[0] = line0;
    edit_color_screen.lens[0] = 8;

    const u8 * const line1 = TXT(TXT_BLUE + SDATA->index);
    edit_color_screen.lines[1] = line1;
    edit_color_screen.lens[1] = strnlen(line1, 8);

    G_DRAWSCREEN(edit_color_screen);
}

/*
 * state prolog
 */
PROLOG()
{
    SDATA->timeout = TIMEOUT;
    if(config.dev_common.night_bri == 0){
		led_addr_set_bri(3);
    }
	SDATA->index = config.dev_common.night_color;
	if(SDATA->index >= LED_COUNT){
		SDATA->index = 0;
	}
	update();
}

/*
 * state epilog
 */
EPILOG()
{
	/* if lang changed, write new lang to config */
	if(SDATA->index != config.dev_common.night_color){
		config.dev_common.night_color = SDATA->index;
		cfg_storage_sync();
	}
    led_addr_set_bri(config.dev_common.night_bri);
    led_addr_off();
}

/*
 * exit state
 */
ON_ESCAPE_P()
{
	STATE_JMP(&main_menu_st);
}

/*
 * exit state
 */
ON_ENTER_P()
{
	STATE_JMP(&main_menu_st);
}

ON_RIGHT_P()
{
	if(SDATA->index == (LED_COUNT - 1)){
		SDATA->index = 0;
	}else{
		SDATA->index++;
	}
	update();
}

ON_LEFT_P()
{
	if(0 == SDATA->index){
		SDATA->index = LED_COUNT - 1;
	}else{
		SDATA->index--;
	}
	update();
}

ON_HSEC()
{
	if(SDATA->timeout){
		--SDATA->timeout;
	}else{
		STATE_JMP(&main_menu_st);
	}
}


INE_STATE(STATE_NAME, 7)
{
	.count = 7,
    .block[0] = PROLOG_EVENT,
    .block[1] = EPILOG_EVENT,
    .block[2] = HSEC_EVENT,
    .block[3] = ESCAPE_P_EVENT,
    .block[4] = ENTER_P_EVENT,
    .block[5] = LEFT_P_EVENT,
    .block[6] = RIGHT_P_EVENT,
};

