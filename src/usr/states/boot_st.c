#define BOOT_ST
#define STATE_NAME 		boot_st
#include "app.h"

#define BOOT_TIMEOUT	(5)

typedef struct {
	u8 timeout;
} sdata_t;
SDATA_DEF;

GSC_SCREEN(boot, "Пульсар3", "Daz_mini")

PROLOG()
{
	upower_put_com(&upower0, UPOWERC_MONITORING);
	msleep(1000);
	SDATA->timeout = BOOT_TIMEOUT;

	const b8 power_is_good = upower_is_good(&upower0);
	if(false == power_is_good){
		STATE_JMP(&power_off_st);
		return;
	}
	BOOSTER5V_ON();
	msleep(50);

    graphic_clear(&graphics);
	G_DRAWSCREEN(boot_screen);
	graphic_on(&graphics);
	net0_on();
}

EPILOG()
{
	graphic_clear(&graphics);
}

ON_SEC()
{
	if(0 != SDATA->timeout){
		SDATA->timeout--;
		if(0 == SDATA->timeout){
			STATE_JMP(&standby_st);
		}
	}
}

INE_STATE(STATE_NAME, 3) {
	.count = 3,
	.block[0] = PROLOG_EVENT,
	.block[1] = EPILOG_EVENT,
	.block[2] = SEC_EVENT,
};
