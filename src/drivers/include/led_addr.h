#ifndef LED_ADDR_H
#define LED_ADDR_H
#include "ia_arch.h"
#include "dport.h"

typedef struct {
    DPORT port;
    u8 pin;
} led_addr_conf_t;

#define CMV   (20)
#define CMVH   (CMV / 3)

#define R_POS   (8)
#define G_POS   (16)
#define B_POS   (0)

#define RGB(__R__, __G__, __B__)    (((__B__ & 0xff) << B_POS) | ((__R__ & 0xff) << R_POS) | ((__G__ & 0xff) << G_POS))

typedef enum __packed {
    BLACK 	= RGB(0, 0, 0),
    WHITE 	= RGB(CMV, CMV, CMV),

    RED 	= RGB(CMV, 0, 0),
    GREEN 	= RGB(0, CMV, 0),
    BLUE 	= RGB(0, 0, CMV),
    
    YELLOW 	= RGB(CMV, CMV, 0),
    MAGANTA = RGB(CMV, 0, CMV),
    CYAN 	= RGB(0, CMV, CMV),

	/* CC5500 */
    ORANGE 	= RGB(CMV, CMVH, 0),



	BLUE_P = 0,
	GREEN_P,
	CYAN_P,
	RED_P,
	MAGANTA_P,
	YELLOW_P,
	ORANGE_P,
	WHITE_P,
	BLACK_P,
	LED_COUNT,
	LED_COUNT_WITHOUT_BLACK = BLACK_P,
} color_t;

extern
const color_t color_list[LED_COUNT];

extern
void led_addr_init(const led_addr_conf_t * const conf);

extern
__attribute__ ((optimize(0)))
void led_addr_set(const color_t color);

extern
void led_addr_off(void);

extern
void led_addr_set_bri(const u8 new_bri);

#endif//LED_ADDR_H

#if defined(DEVICE_TREE_H)
/* nothing to extern */
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(LED_ADDR_DEVICE_TREE)
#define LED_ADDR_DEVICE_TREE
static const
led_addr_conf_t led_addr_conf = {
    .port = LED_ADDR_PORT,
    .pin = LED_ADDR_PIN,
};
#define LED_ADDR_INIT()		    led_addr_init(&led_addr_conf)
#endif
