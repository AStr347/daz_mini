#define NET_DEV_EDIT_NAME_ST
#define STATE_NAME		dedit_name_st

#include "app.h"
/*
 * current_edit_state=edit_state_brightness;
 */
typedef struct {
	DEV_INFO_T * restrict const edev;
	u8 line_num;
	u16 timeout;
	u8 index;
}state_data_t;
static state_data_t* SDATA = (state_data_t*)data_overlay;

/*
 * state prolog
 */
PROLOG()
{
	SDATA->timeout = TIMEOUT;
    SDATA->index = SDATA->edev->color;
}

/*
 * state epilog
 */
EPILOG()
{
}

/*
 * exit state
 */
ON_ESCAPE_P() { STATE_JMP(&net_edit_dev_st); }

ON_SEC()
{
	if(SDATA->timeout){
		--SDATA->timeout;
	}else{
		STATE_JMP(&standby_st);
	}
}

INE_STATE(STATE_NAME, 4)
{
	.count = 4,
    .block[0] = PROLOG_EVENT,
    .block[1] = EPILOG_EVENT,
    .block[2] = SEC_EVENT,
    .block[3] = ESCAPE_P_EVENT,
};
