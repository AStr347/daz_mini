#define DEVICE_TREE_MODULE
#include "ia_arch.h"
#include "dport.h"
#include "app.h"

#include "device_tree.h"

static
void remap_init(void){
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
}

static
void device_tree_init(void){
	DMA_INIT();
	ADC_INIT();
	EXTINT_INIT();

	SPI1_INIT();
	SPI2_INIT();

	TIMER1_INIT();
	TIMER2_INIT();
	TIMER3_INIT();
	TIMER4_INIT();

	IC1_INIT();
	IC2_INIT();
	IC3_INIT();
	IC4_INIT();

	PWM1_INIT();
	PWM2_INIT();
	PWM3_INIT();
	PWM4_INIT();

	INEVENT_INIT();
	RTC_INIT();
	KEYPAD_INIT();

	/* need for graphic and led_addr */
	BOOSTER5V_INIT();

	GRAPHIC_INIT();
	LED_ADDR_INIT();
	SSIGN_INIT();

	SND_INIT();
	NET_LEGACY_INIT();

	UPOWER_INIT();

	LOGE("!!! device_tree inited !!!");
}

int main(void)
{
    SystemCoreClockUpdate();
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    DPORT_DIN_INIT(GPIOA, 0xffff);
    DPORT_DIN_INIT(GPIOB, 0xffff);
    DPORT_DIN_INIT(GPIOC, 0xffff);

    LOG_INIT();

    remap_init();
    device_tree_init();
	app_init();

    vTaskStartScheduler();
    for (;;) {
        /* Normally should not reach this point */
    };
}

