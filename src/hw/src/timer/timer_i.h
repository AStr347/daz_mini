#ifndef TIMER_MODULE
#error "must be include just into timer.c"
#endif
#include "timer.h"

#define THIS ia_timer_t * const this

static
TIMER const timers[4] = {
    [0] = TIM1,
    [1] = TIM2,
    [2] = TIM3,
    [3] = TIM4,
};

static const
u16 num2apb[4] = {
    [0] = RCC_APB2Periph_TIM1,
    [1] = RCC_APB1Periph_TIM2,
    [2] = RCC_APB1Periph_TIM3,
    [3] = RCC_APB1Periph_TIM4,
};

#define TIMER_CH0_OFFSET    (offsetof(TIM_TypeDef, CH1CVR))
#define TIMER_CH_STEP       (4)

static const
dma_chnum_t timer_numch2dma_ch[] = {
    [0] = DMA_TIM1_UP,
    [1] = DMA_TIM2_UP,
    [2] = DMA_TIM3_UP,
    [3] = DMA_TIM4_UP,
};

static const
u16 timer_ch2dmabase[] = {
    [0] = TIM_DMABase_CCR1,
    [1] = TIM_DMABase_CCR2,
    [2] = TIM_DMABase_CCR3,
    [3] = TIM_DMABase_CCR4,
};

static
void * const timer_numch2dataptr[] = {
    [0] = (void * const)(&TIM1->CH1CVR),
    [1] = (void * const)(&TIM1->CH2CVR),
    [2] = (void * const)(&TIM1->CH3CVR),
    [3] = (void * const)(&TIM1->CH4CVR),

	[4] = (void * const)(&TIM2->CH1CVR),
    [5] = (void * const)(&TIM2->CH2CVR),
    [6] = (void * const)(&TIM2->CH3CVR),
    [7] = (void * const)(&TIM2->CH4CVR),

    [8] = (void * const)(&TIM3->CH1CVR),
    [9] = (void * const)(&TIM3->CH2CVR),
    [10] = (void * const)(&TIM3->CH3CVR),
    [11] = (void * const)(&TIM3->CH4CVR),

    [12] = (void * const)(&TIM4->CH1CVR),
    [13] = (void * const)(&TIM4->CH2CVR),
    [14] = (void * const)(&TIM4->CH3CVR),
    [15] = (void * const)(&TIM4->CH4CVR),
};

static const
IRQn_Type timer_irqs[] = {
	[0] = TIM1_UP_IRQn,
	[1] = TIM2_IRQn,
	[2] = TIM3_IRQn,
	[3] = TIM4_IRQn,
};

static const
u16 divs[TPSET_CNT] = {
	[TPSET_OC_MAXIMUM_1PER] = TIM_CKD_DIV1,
	[TPSET_OC_250NS_1PER] = TIM_CKD_DIV1,
	[TPSET_OC_1US_1PER] = TIM_CKD_DIV4,
	[TPSET_OC_10US_1PER] = TIM_CKD_DIV4,
	[TPSET_OC_50US_1PER] = TIM_CKD_DIV4,

	[TPSET_IC_MAXIMUM] = TIM_CKD_DIV1,
	[TPSET_IC_1US] = TIM_CKD_DIV4,
};

static const
u16 pscs[TPSET_CNT] = {
	[TPSET_OC_MAXIMUM_1PER] = 0,
	[TPSET_OC_250NS_1PER] = 0,
	[TPSET_OC_1US_1PER] = 23,
	[TPSET_OC_10US_1PER] = 239,
	[TPSET_OC_50US_1PER] = 1199,

	[TPSET_IC_MAXIMUM] = 0,
	[TPSET_IC_1US] = 47,
};

static const
u16 pers[TPSET_CNT] = {
	[TPSET_OC_MAXIMUM_1PER] = 1,
	[TPSET_OC_250NS_1PER] = 1,
	[TPSET_OC_1US_1PER] = 1,
	[TPSET_OC_10US_1PER] = 1,
	[TPSET_OC_50US_1PER] = 1,

	[TPSET_IC_MAXIMUM] = 0xffff,
	[TPSET_IC_1US] = 0x7fff, /* not use highest bit because slice_t store level here */
};

