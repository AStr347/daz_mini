#define EXTINT_MODULE
#include "extint_i.h"

typedef struct {
    tdc_t waiter;
} exti_ctx_t;

exti_ctx_t exti_ctx0;

/**
 * init exti_ctx0 object
 */
void extint_init(void){
	memclr(&exti_ctx0, sizeof(exti_ctx_t));
    cinit(exti_ctx0.waiter);

    /* disable all EXTI's and EXTI IRQ's */
	NVIC_InitTypeDef exti_nvic = {
		.NVIC_IRQChannel = 0,
		.Priority = 3,
		.NVIC_IRQChannelCmd = DISABLE,
	};
    exti_nvic.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Init(&exti_nvic);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);

    exti_nvic.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&exti_nvic);
    NVIC_ClearPendingIRQ(EXTI1_IRQn);

    exti_nvic.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&exti_nvic);
    NVIC_ClearPendingIRQ(EXTI2_IRQn);

    exti_nvic.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_Init(&exti_nvic);
    NVIC_ClearPendingIRQ(EXTI3_IRQn);

    exti_nvic.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_Init(&exti_nvic);
    NVIC_ClearPendingIRQ(EXTI4_IRQn);

    exti_nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Init(&exti_nvic);
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);

    exti_nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Init(&exti_nvic);
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);

    EXTI_InitTypeDef exti_init = {
		.EXTI_Line = 0,
		.EXTI_LineCmd = DISABLE,
		.EXTI_Mode = EXTI_Mode_Interrupt,
		.EXTI_Trigger = 0,
	};
    for(u8 i = 0; i < 16; i++){
    	const u16 mask = (1 << i);
        exti_init.EXTI_Line = mask;
        EXTI_Init(&exti_init);
    	EXTI_ClearFlag(mask);
    }
}

/**
 * wait event from irq group
 * @param group - group index to wait
 * @param time - maximum timeout to wait
 * 
 * @return true - waited
 *         false - timeout
 */
b8 extint_wait(const exti_group_t groups, const u32 time){
    const u32 waited = xEventGroupWaitBits(exti_ctx0.waiter.cond,
										   groups,
										   true,
										   false,
										   time);
    return (0 != waited);
}

/**
 * configure EXTI registers for port and pin IRQ
 * @param port - GPIO[A,B,C,D,E,F]
 * @param pin  - number of pin [0..15]
 * @param trig - trigger type [falling, rising, any] edge
 */
exti_group_t extint_configure(const DPORT port,
                              const u8 pin,
                              const EXTITrigger_TypeDef trig)
{
    if(pin > 15){
        return 0;
    }
    switch ((u32)port)
    {
        case ((u32)GPIOA):{
            GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, pin);
        } break;

        case ((u32)GPIOB):{
            GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, pin);
        } break;

        case ((u32)GPIOC):{
            GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, pin);
        } break;

        case ((u32)GPIOD):{
            GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, pin);
        } break;

        case ((u32)GPIOE):{
            GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, pin);
        } break;

        case ((u32)GPIOF):{
            GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, pin);
        } break;

        case ((u32)GPIOG):{
            GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, pin);
        } break;

        default:{
            return 0;
        } break;
    }

    const exti_group_t mask = (1 << pin);
    const EXTI_InitTypeDef exti_init = {
        .EXTI_Line = mask,
        .EXTI_LineCmd = ENABLE,
        .EXTI_Mode = EXTI_Mode_Interrupt,
        .EXTI_Trigger = trig,
    };
    EXTI_Init(&exti_init);
    

    const NVIC_InitTypeDef exti_nvic = {
        .NVIC_IRQChannel = extint_nvic_irqs[pin],
        .Priority = 0xf0,
        .NVIC_IRQChannelCmd = ENABLE,
    };
    NVIC_Init(&exti_nvic);

    return mask;
}


/**
 * IRQ handlers
 * wake evntgroup bits and clear irq flags
 */


__attribute__((interrupt()))
void EXTI0_IRQHandler(void){
	EXTI_ClearFlag(EXTI_Line0);
    BaseType_t bogus = 0;
    xEventGroupSetBitsFromISR(exti_ctx0.waiter.cond, EXTIG_0_BIT, &bogus);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
}

__attribute__((interrupt()))
void EXTI1_IRQHandler(void){
	EXTI_ClearFlag(EXTI_Line1);
    BaseType_t bogus = 0;
    xEventGroupSetBitsFromISR(exti_ctx0.waiter.cond, EXTIG_1_BIT, &bogus);
    NVIC_ClearPendingIRQ(EXTI1_IRQn);
}

__attribute__((interrupt()))
void EXTI2_IRQHandler(void){
	EXTI_ClearFlag(EXTI_Line2);
    BaseType_t bogus = 0;
    xEventGroupSetBitsFromISR(exti_ctx0.waiter.cond, EXTIG_2_BIT, &bogus);
    NVIC_ClearPendingIRQ(EXTI2_IRQn);
}

__attribute__((interrupt()))
void EXTI3_IRQHandler(void){
	EXTI_ClearFlag(EXTI_Line3);
    BaseType_t bogus = 0;
    xEventGroupSetBitsFromISR(exti_ctx0.waiter.cond, EXTIG_3_BIT, &bogus);
    NVIC_ClearPendingIRQ(EXTI3_IRQn);
}

__attribute__((interrupt()))
void EXTI4_IRQHandler(void){
	EXTI_ClearFlag(EXTI_Line4);
    BaseType_t bogus = 0;
    xEventGroupSetBitsFromISR(exti_ctx0.waiter.cond, EXTIG_4_BIT, &bogus);
    NVIC_ClearPendingIRQ(EXTI4_IRQn);
}

__attribute__((interrupt()))
void EXTI9_5_IRQHandler(void){
	const u16 flags = EXTI->INTFR & (EXTIG_5_9_M);
	EXTI_ClearFlag(flags);
    BaseType_t bogus = 0;
    xEventGroupSetBitsFromISR(exti_ctx0.waiter.cond, flags, &bogus);
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
}

__attribute__((interrupt()))
void EXTI15_10_IRQHandler(void){
	const u16 flags = EXTI->INTFR & (EXTIG_10_15_M);
	EXTI_ClearFlag(flags);
	BaseType_t bogus = 0;
	xEventGroupSetBitsFromISR(exti_ctx0.waiter.cond, flags, &bogus);
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
}
