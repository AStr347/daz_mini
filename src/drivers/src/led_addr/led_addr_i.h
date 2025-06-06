#ifndef LED_ADDR_MODULE
#error "must be include just into led_addr.c"
#endif
#include "led_addr.h"

#define u04sleep(void)\
__asm__ volatile ("nop");/*1*/\
__asm__ volatile ("nop");/*2*/\
__asm__ volatile ("nop");/*3*/\
__asm__ volatile ("nop");/*4*/\
__asm__ volatile ("nop");/*5*/\
__asm__ volatile ("nop");/*6*/\
__asm__ volatile ("nop");/*7*/\
__asm__ volatile ("nop");/*8*/\
__asm__ volatile ("nop");/*9*/

// __asm__ volatile ("nop");/*10*/\
// __asm__ volatile ("nop");/*11*/\
// __asm__ volatile ("nop");/*12*/\
// __asm__ volatile ("nop");/*13*/\
// __asm__ volatile ("nop");/*14*/\


#define u08sleep()\
__asm__ volatile ("nop");/*1*/\
__asm__ volatile ("nop");/*2*/\
__asm__ volatile ("nop");/*3*/\
__asm__ volatile ("nop");/*4*/\
__asm__ volatile ("nop");/*5*/\
__asm__ volatile ("nop");/*6*/\
__asm__ volatile ("nop");/*7*/\
__asm__ volatile ("nop");/*8*/\
__asm__ volatile ("nop");/*9*/\
__asm__ volatile ("nop");/*10*/\
__asm__ volatile ("nop");/*11*/\
__asm__ volatile ("nop");/*12*/\
__asm__ volatile ("nop");/*13*/\
__asm__ volatile ("nop");/*14*/\
__asm__ volatile ("nop");/*15*/\
__asm__ volatile ("nop");/*16*/\
__asm__ volatile ("nop");/*17*/\
__asm__ volatile ("nop");/*18*/\
__asm__ volatile ("nop");/*19*/\
__asm__ volatile ("nop");/*20*/\

// __asm__ volatile ("nop");/*21*/\
// __asm__ volatile ("nop");/*22*/\
// __asm__ volatile ("nop");/*23*/\
// __asm__ volatile ("nop");/*24*/\
// __asm__ volatile ("nop");/*25*/\
// __asm__ volatile ("nop");/*26*/\
// __asm__ volatile ("nop");/*27*/\
// __asm__ volatile ("nop");/*28*/\
// __asm__ volatile ("nop");/*29*/\
// __asm__ volatile ("nop");/*30*/\
// __asm__ volatile ("nop");/*31*/\
// __asm__ volatile ("nop");/*32*/\
// __asm__ volatile ("nop");/*33*/\
// __asm__ volatile ("nop");/*34*/\
// __asm__ volatile ("nop");/*35*/\
// __asm__ volatile ("nop");/*36*/\
// __asm__ volatile ("nop");/*37*/\
// __asm__ volatile ("nop");/*38*/\
// __asm__ volatile ("nop");/*39*/\
// __asm__ volatile ("nop");/*40*/\
// __asm__ volatile ("nop");/*41*/\
// __asm__ volatile ("nop");/*42*/\
// __asm__ volatile ("nop");/*43*/\
// __asm__ volatile ("nop");/*44*/\
// __asm__ volatile ("nop");/*45*/\
// __asm__ volatile ("nop");/*46*/\
// __asm__ volatile ("nop");/*47*/\
// __asm__ volatile ("nop");/*48*/\


#define u045sleep(void)\
__asm__ volatile ("nop");/*1*/\
__asm__ volatile ("nop");/*2*/\
__asm__ volatile ("nop");/*3*/\
__asm__ volatile ("nop");/*4*/\
__asm__ volatile ("nop");/*5*/\
__asm__ volatile ("nop");/*6*/\
__asm__ volatile ("nop");/*7*/\
__asm__ volatile ("nop");/*8*/\
__asm__ volatile ("nop");/*9*/\
__asm__ volatile ("nop");/*10*/


// __asm__ volatile ("nop");/*11*/\
// __asm__ volatile ("nop");/*12*/\
// __asm__ volatile ("nop");/*13*/\
// __asm__ volatile ("nop");/*14*/\
// __asm__ volatile ("nop");/*15*/\
// __asm__ volatile ("nop");/*16*/\


#define u085sleep()\
__asm__ volatile ("nop");/*1*/\
__asm__ volatile ("nop");/*2*/\
__asm__ volatile ("nop");/*3*/\
__asm__ volatile ("nop");/*4*/\
__asm__ volatile ("nop");/*5*/\
__asm__ volatile ("nop");/*6*/\
__asm__ volatile ("nop");/*7*/\
__asm__ volatile ("nop");/*8*/\
__asm__ volatile ("nop");/*9*/\
__asm__ volatile ("nop");/*10*/\
__asm__ volatile ("nop");/*11*/\
__asm__ volatile ("nop");/*12*/\
__asm__ volatile ("nop");/*13*/\
__asm__ volatile ("nop");/*14*/\
__asm__ volatile ("nop");/*15*/\
__asm__ volatile ("nop");/*16*/\
__asm__ volatile ("nop");/*17*/\
__asm__ volatile ("nop");/*18*/\
__asm__ volatile ("nop");/*19*/\
__asm__ volatile ("nop");/*20*/\
__asm__ volatile ("nop");/*21*/\
__asm__ volatile ("nop");/*22*/\
__asm__ volatile ("nop");/*23*/\
__asm__ volatile ("nop");/*24*/\
__asm__ volatile ("nop");/*25*/\
__asm__ volatile ("nop");/*26*/\
__asm__ volatile ("nop");/*27*/\
__asm__ volatile ("nop");/*28*/\
__asm__ volatile ("nop");/*29*/\
__asm__ volatile ("nop");/*30*/

// __asm__ volatile ("nop");/*31*/\
// __asm__ volatile ("nop");/*32*/\
// __asm__ volatile ("nop");/*33*/\
// __asm__ volatile ("nop");/*34*/\
// __asm__ volatile ("nop");/*35*/\
// __asm__ volatile ("nop");/*36*/\
// __asm__ volatile ("nop");/*37*/\
// __asm__ volatile ("nop");/*38*/\
// __asm__ volatile ("nop");/*39*/\
// __asm__ volatile ("nop");/*40*/\
// __asm__ volatile ("nop");/*41*/\
// __asm__ volatile ("nop");/*42*/\
// __asm__ volatile ("nop");/*43*/\
// __asm__ volatile ("nop");/*44*/\
// __asm__ volatile ("nop");/*45*/\


const color_t color_list[LED_COUNT] = {
    [BLUE_P] = BLUE,
	[GREEN_P] = GREEN,
	[CYAN_P] = CYAN,
	[RED_P] = RED,
	[MAGANTA_P] = MAGANTA,
	[YELLOW_P] = YELLOW,
	[ORANGE_P] = ORANGE,
	[WHITE_P] = WHITE,
	[BLACK_P] = BLACK,
};

