#define DEVICE_TREE_MODULE
#include "ia_arch.h"
#include "dport.h"
#include "app.h"

#include "device_tree.h"

#define TEST_TASK_STACK_SIZ 	(512)

#define TEST_PORT 	GPIOB
#define TEST_MASK	(1 << 1)

typedef struct {
	tdc_t cond;
    TASK_T(TEST_TASK_STACK_SIZ);
} test_t;
test_t test;

void test_task(void){
	DPORT_DOUT_INIT(BCB0802_D0_PORT, (1 << BCB0802_DO_PIN));

	net0_off();
	msleep(3000);
	net0_on();
	net0_beh_set(NETB_TRX);
	net0_mac_set(0x1844000011223344);
	net0_normal();

    for(;;){
		msleep(3000);
    }
}


static
void remap_init(void){
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
}

int main(void)
{
    SystemCoreClockUpdate();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    DPORT_DIN_INIT(GPIOA, 0xffff);
    DPORT_DIN_INIT(GPIOB, 0xffff);
    DPORT_DIN_INIT(GPIOC, 0xffff);

    remap_init();

    LOG_INIT();

    // device_tree_init();
	// app_init();
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
	NET_LEGACY_INIT();

	cinit(test.cond);

    TaskCreate(&test, test_task, TEST_TASK_STACK_SIZ, 4);

    vTaskStartScheduler();
    for (;;) {
        /* Normally should not reach this point */
    };
}

