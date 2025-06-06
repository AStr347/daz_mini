#define ALARM_MELODY_ST
/*
 * edit long interval
 */
#define STATE_NAME		alarm_melody_st

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
static u8 long_str[8] = "   XX   ";
GS_SCREEN(edit_melody, NULL, long_str);

/**
 * prbar contain real value of led bri
 */
static
void update(void)
{
    SDATA->timeout = TIMEOUT;
    
    const u8 value = SDATA->index + 1;
    u8 * dst = long_str + LONG_POS;
	*dst++ = '0' + (value / 10);
    *dst++ = '0' + (value % 10);

    const u8 * const line0 = TXT(TXT_AM_MELODY);
    edit_melody_screen.lines[0] = line0;
    edit_melody_screen.lens[0] = 8;

    const u8 * const line1 = long_str;
    edit_melody_screen.lines[1] = line1;
    edit_melody_screen.lens[1] = 8;

    G_DRAWSCREEN(edit_melody_screen);
    buzzer_play(BUZ_ALARM1 + SDATA->index, 1);
}

/*
 * state prolog
 */
PROLOG()
{
	SDATA->timeout = TIMEOUT;
    SDATA->index = config.dev_common.alarm_melody;
	update();
}

/*
 * state epilog
 */
EPILOG()
{
	if(SDATA->index != config.dev_common.alarm_melody){
		config.dev_common.alarm_melody = SDATA->index;
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
	if(SDATA->index < 2){
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
        
