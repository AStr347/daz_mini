#define LED_ADDR_MODULE
#include "led_addr_i.h"

typedef struct {
    DPORT port;
    u16 mask;
    u8 bri;
    color_t last_color;
    tdm_t mutex;
} led_t;
static
led_t led0;

static
void u24_transmit(const u32 data){
    for(u8 i = 23; i != 0xff; i--){
        if((data & (1L << i))){
            DPORT_SET(led0.port, led0.mask);
            /* delay 0.85 us */
            u085sleep();
            DPORT_CLR(led0.port, led0.mask);
            /* delay 0.45 us */
            u045sleep();
        } else {
            DPORT_SET(led0.port, led0.mask);
            /* delay 0.4 us */
            u04sleep();
            DPORT_CLR(led0.port, led0.mask);
            /* delay 0.8 us */
            u08sleep();
        }
    }
}

void led_addr_init(const led_addr_conf_t * const conf){
	memclr(&led0, sizeof(led_t));
    minit(led0.mutex);
    led0.port = conf->port;
    led0.mask = (1 << conf->pin);
    led0.bri = 0xff;
    led0.last_color = WHITE;

    DPORT_DOUT_INIT(led0.port, led0.mask);
}

__attribute__ ((optimize(0)))
void led_addr_set(const color_t color){
    mlock(led0.mutex);
	if(color == led0.last_color){
        mulock(led0.mutex);
		return;
	}
	DPORT_CLR(led0.port, led0.mask);
	usleep(50);
	taskENTER_CRITICAL();

	for(u8 i = 0; i < LED_ADDR_COUNT; i++){
		u24_transmit(color);
	}

    taskEXIT_CRITICAL();
    DPORT_CLR(led0.port, led0.mask);
	usleep(50);
	led0.last_color = color;
    mulock(led0.mutex);
}

void led_addr_off(void){
    led_addr_set(BLACK);
}

void led_addr_set_bri(const u8 new_bri){
	led0.bri = new_bri;
	/* may be led_addr_set */
}

