#define NET_DEV_EDIT_MELODY_ST
#define STATE_NAME		dedit_melody_st

#include "app.h"
/*
 * current_edit_state=edit_state_brightness; 
 */
typedef struct {
	DEV_INFO_T * restrict const edev;
	u16 timeout;
	u8 index;
}state_data_t;
static state_data_t* SDATA = (state_data_t*)data_overlay;

#define LONG_POS     (3)
static u8 long_str[8] = "   XX   ";
GS_SCREEN(dedit_melody, NULL, long_str);

/**
 * prbar contain real value of led bri
 */
static
void update(void)
{
    SDATA->timeout = TIMEOUT;
    
    const u8 value = SDATA->index;
    u8 * dst = long_str + LONG_POS;
	*dst++ = '0' + (value / 10);
    *dst++ = '0' + (value % 10);

    const u8 * const line0 = TXT(TXT_NMED_MELODY);
    dedit_melody_screen.lines[0] = line0;
    dedit_melody_screen.lens[0] = 8;

    const u8 * const line1 = long_str;
    dedit_melody_screen.lines[1] = line1;
    dedit_melody_screen.lens[1] = 0;

    G_DRAWSCREEN(dedit_melody_screen);
}

/*
 * state prolog
 */
PROLOG()
{
	SDATA->timeout = TIMEOUT;
	SDATA->index = SDATA->edev->melody;
	if (config.dev_common.vol == 0){
		buzzer_volume_set(3);
    }
	SDATA->timeout = TIMEOUT;
	update();
}

/*
 * state epilog
 */
EPILOG()
{
	buzzer_stop();
	if (config.dev_common.vol == 0){
		buzzer_volume_set(config.dev_common.vol);
    }
	if(SDATA->index != SDATA->edev->melody){
		SDATA->edev->melody = SDATA->index;
		cfg_storage_sync();
	}
}

/*
 * exit state
 */
ON_ESCAPE_P() { STATE_JMP(&net_edit_dev_st); }

/*
 * exit state
 */
ON_ENTER_P() { STATE_JMP(&net_edit_dev_st); }

ON_LEFT_P()
{
	SDATA->timeout = TIMEOUT;
	if(0 == SDATA->index){
        SDATA->index = (BUZ_COUNT - 1);
	}else{
		--SDATA->index;
	}
	update();
}

ON_RIGHT_P()
{
	SDATA->timeout = TIMEOUT;
	if((BUZ_COUNT - 1) == SDATA->index){
		SDATA->index = 0;
	}else{
		++SDATA->index;
	}
    update();
}

ON_SEC()
{
	if(SDATA->timeout){
		--SDATA->timeout;
	}else{
		STATE_JMP(&standby_st);
	}
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