#define EDIT_LONG_SIGNAL_ST
/*
 * edit long interval
 */
#define STATE_NAME		edit_long_signal_st

#include "app.h"

/*
 * current_edit_state=edit_state_brightness; 
 */
typedef struct {
	u8 timeout;
	u8 index;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

#define LONG_POS     (3)
static u8 long_str[8] = "   XX";
GS_SCREEN(edit_long, NULL, long_str);

/**
 * prbar contain real value of led bri
 */
static
void update(void)
{
	SDATA->timeout = TIMEOUT;

    const u8 value = long_to_config_translator[SDATA->index];
    u8 * dst = long_str + LONG_POS;
	*dst++ = '0' + (value / 10);
    *dst++ = '0' + (value % 10);

    const u8 * const line0 = TXT(TXT_MM_VOL);
    edit_long_screen.lines[0] = line0;
    edit_long_screen.lens[0] = 8;

    const u8 * const line1 = long_str;
    edit_long_screen.lines[1] = line1;
    edit_long_screen.lens[1] = strnlen(line1, 8);

    G_DRAWSCREEN(edit_long_screen);
}

/*
 * state prolog
 */
PROLOG()
{
	SDATA->timeout = TIMEOUT;
	if(config.dev_common.long_signal < LONG_COUNT){
		SDATA->index = config.dev_common.long_signal;
	}else{
		SDATA->index = 0;
	}
	update();
}

/*
 * state epilog
 */
EPILOG()
{
	if(SDATA->index != config.dev_common.long_signal){
		config.dev_common.long_signal = SDATA->index;
		cfg_storage_sync();
	}
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

/*
 * increase brightness
 */
ON_RIGHT_P()
{
	if(SDATA->index < LONG_COUNT - 1){
		SDATA->index++;
		update();
	}
}

/*
 * decrease brightness
 */
ON_LEFT_P()
{
	if(0 < SDATA->index){
		SDATA->index--;
		update();
	}
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
        