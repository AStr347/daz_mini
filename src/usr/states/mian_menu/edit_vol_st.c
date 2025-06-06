#define EDIT_VOL_ST
/*
 * edit system volume
 */
#define STATE_NAME		edit_vol_st

#include "app.h"

typedef struct {
	u8 timeout;
	u8 index;
    b8 pressed;
} state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

#define VOL_POS     (4)
u8 vol_str[8] = "    X   ";
GS_SCREEN(edit_vol, NULL, vol_str);

/**
 * prbar contain real value of led bri
 */
static
void update(void){
	SDATA->timeout = TIMEOUT;

    buzzer_volume_set(SDATA->index);
    vol_str[VOL_POS] = '0' + SDATA->index;

    const u8 * const line1 = vol_str;
    edit_vol_screen.lines[1] = line1;
    edit_vol_screen.lens[1] = strnlen(line1, 8);

    G_DRAWLINE(edit_vol_screen, 1);
	buzzer_play(BUZ_TEST, 1);
}

/*
 * state prolog
 */
PROLOG()
{
    SDATA->timeout = TIMEOUT;
	
    if(4 <= config.dev_common.vol){
        SDATA->index = config.dev_common.vol = 3;
    } else {
        SDATA->index = config.dev_common.vol;
    }
    const u8 * const line0 = TXT(TXT_MM_VOL);
	edit_vol_screen.lines[0] = line0;
	edit_vol_screen.lens[0] = 8;
	G_DRAWLINE(edit_vol_screen, 0);
	update();
}

/*
 * state epilog
 */
EPILOG()
{
	buzzer_stop();
	if(SDATA->index != config.dev_common.vol){
		config.dev_common.vol = SDATA->index;
		cfg_storage_sync();
	}
}

/*
 * exit state
 */
ON_ENTER_P() { STATE_JMP(&main_menu_st); }

/*
 * exit state
 */
ON_ESCAPE_P() { STATE_JMP(&main_menu_st); }

/*
 * increase volume
 */
static
void inc(void){
    if(3 > SDATA->index){
        ++SDATA->index;
		update();
	}
}

/*
 * decrease volume
 */
static
void dec(void){
    if(0 < SDATA->index){
        --SDATA->index;
		update();
	}
}

ON_LEFT_P(){ dec(); }

ON_RIGHT_P(){ inc(); }

ON_LEFT_H(const u8 time){
    if(BASE_LONG_HOLD < time){
        dec();
    }
}

ON_RIGHT_H(const u8 time){
    if(BASE_LONG_HOLD < time){
        inc();
    }
}

ON_SEC(){
    if(0 == SDATA->timeout){
        STATE_JMP(&main_menu_st);
    } else {
        SDATA->timeout--;
    }
}

INE_STATE(STATE_NAME, 9)
{
	.count = 9,
    .block[0] = PROLOG_EVENT,
    .block[1] = EPILOG_EVENT,
    .block[2] = SEC_EVENT,
    .block[3] = ESCAPE_P_EVENT,
    .block[4] = ENTER_P_EVENT,
    .block[5] = LEFT_P_EVENT,
    .block[6] = RIGHT_P_EVENT,
    .block[7] = LEFT_H_EVENT,
    .block[8] = RIGHT_H_EVENT,
};
