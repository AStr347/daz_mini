#define POWER_OFF_ST
#define STATE_NAME 		power_off_st
#include "app.h"

PROLOG()
{
    net0_off();
    led_addr_off();
    snd_sound_stop(&snd0);
    //lstop();
    //mstop();
    graphic_clear(&graphics);
    graphic_off(&graphics);
    msleep(50);
    BOOSTER5V_OFF();
}

EPILOG()
{
	
}

ON_ESCAPE_H(const u8 time)
{
    if(20 == time){
        STATE_JMP(&boot_st);
    }
}

INE_STATE(STATE_NAME, 3) {
	.count = 3,
	.block[0] = PROLOG_EVENT,
	.block[1] = EPILOG_EVENT,
	.block[2] = ESCAPE_H_EVENT,
};
