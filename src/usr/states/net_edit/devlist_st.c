#define NET_DEVLIST_ST
/**
 * notify edit menu for table receiver
 * here we chouse the sensor for edit from connected sensor list
 */

#define STATE_NAME		devlist_st

#include "app.h"

/**
 * info_sel is selected to edit entry
 * so we will use it in other edit states
 */
typedef struct {
	DEV_INFO_T * info_sel;
	u16 timeout;
	u8 sel;
	u8 max;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

static u8 name_str[8] = "NNNNNNNN";
GS_SCREEN(devlist, NULL, name_str)

/* update draw information with new values
 * todo: we think that new_text length is always 8 symbols
 * @param sel		- dev type 
 **/
static
void update(void)
{
    SDATA->timeout = TIMEOUT;

    const u8 * const line0 = TXT(TXT_NMED_CHOOSE);
    devlist_screen.lines[0] = line0;
    devlist_screen.lens[0] = 8;

    /* todo: name from device info */
    const u8 * const line1 = TXT(TXT_NMED_CHOOSE);
    devlist_screen.lines[1] = line1;
    devlist_screen.lens[1] = strnlen(line1, 8);

	G_DRAWSCREEN(devlist_screen);
}

/* */
static
void next(void)
{
	/* check next */
	if(SDATA->sel == SDATA->max - 1){
		/* rollover */
		SDATA->sel = 0;
	}else{
        /* inc sel counter */
		++SDATA->sel;
	}

	SDATA->info_sel = (DEV_INFO_T *)notify_get_index(&config.condev.info, SDATA->sel);
	update();
}

/* */
static
void prev(void)
{
	/* check prev */
	if(0 == SDATA->sel){
        /* rollover */
		SDATA->sel = SDATA->max - 1;
	}else{
		/* dec sel counter */
		--SDATA->sel;
	}

	SDATA->info_sel = (DEV_INFO_T *)notify_get_index(&config.condev.info, SDATA->sel);
	update();
}

/**
 * init draw information
 */
static
void draw_init(void)
{
	/* check count */
	if(SDATA->max == 0){
		/* empty */
		const u8 * const line0 = TXT(TXT_NMED_EMPTY);
        devlist_screen.lines[0] = line0;
        devlist_screen.lens[0] = 8;

        const u8 * const line1 = TXT(TXT_EMPTY);
        devlist_screen.lines[1] = line1;
        devlist_screen.lens[1] = strnlen(line1, 8);

        G_DRAWSCREEN(devlist_screen);
	}else{
		DEV_INFO_T * info_sel = (DEV_INFO_T *)notify_get_index(&config.condev.info, SDATA->sel);
		if(likely(NULL != info_sel)){
            SDATA->info_sel = info_sel;
			update();
		}else{
			STATE_JMP(&standby_st);
		}
	}
}

PROLOG()
{
    SDATA->info_sel = NULL;
	SDATA->timeout = TIMEOUT;
	SDATA->sel = 0;
	SDATA->max = config.condev.info.cnt;

	if(0 == SDATA->max){
        SDATA->timeout = 3;
    }
	draw_init();
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
		STATE_JMP(&net_menu_st);
	}
}

ON_ESCAPE_P() { STATE_JMP(&net_menu_st); }

ON_ENTER_P()
{
	if(SDATA->info_sel){
		STATE_JMP(&net_edit_dev_st);
	}else{
		STATE_JMP(&net_menu_st);
	}
}

ON_LEFT_P()
{
    prev();
}

ON_RIGHT_P()
{
    next();
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


