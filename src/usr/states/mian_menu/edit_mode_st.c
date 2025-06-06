#define EDIT_MODE_ST
/**
 * Date state
 */
#define STATE_NAME		edit_mode_st
#define STATE_GET		SM_J
#define STATE_LEAVE		SM_J
#include "app.h"

typedef struct {
	indcirc_t indcirc;
	u8 timeout;
} state_data_t;
#define SDATA   ((state_data_t*)data_overlay)


GS_SCREEN(edit_mode, NULL, NULL)

static
void update(){
	SDATA->timeout = TIMEOUT;

	const u8 * const line0 = TXT(TXT_MM_NIGHT_COLOR);
    edit_mode_screen.lines[0] = line0;
    edit_mode_screen.lens[0] = 8;

    const u8 * const line1 = TXT(TXT_IS_NONE + SDATA->indcirc);
    edit_mode_screen.lines[1] = line1;
    edit_mode_screen.lens[1] = strnlen(line1, 8);

    G_DRAWSCREEN(edit_mode_screen);
}

PROLOG() {
	SDATA->timeout = TIMEOUT;
	SDATA->indcirc = config.dev_common.indcirc;
	update();
}

EPILOG() {
	graphic_clear(&graphics);
	if(SDATA->indcirc != config.dev_common.indcirc){
		config.dev_common.indcirc = SDATA->indcirc;
	}
}

ON_HSEC(){
	if(SDATA->timeout){
		SDATA->timeout--;
	} else {
		STATE_JMP(&main_menu_st);
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

ON_RIGHT_P()
{
	if(SDATA->indcirc == IS_ALL){
		SDATA->indcirc = IS_NONE;
	}else{
		SDATA->indcirc++;
	}
	update();
}

ON_LEFT_P()
{
	if(SDATA->indcirc == IS_NONE){
		SDATA->indcirc = IS_ALL;
	}else{
		SDATA->indcirc--;
	}
	update();
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
