#include "ic.h"

#define THIS ic_t * const this

static const
u16 ic_ch2regval[4] = {
    [0] = TIM_Channel_1,
    [1] = TIM_Channel_2,
    [2] = TIM_Channel_3,
    [3] = TIM_Channel_4,
};

static const
IRQn_Type ic_irqs[4] = {
	[0] = TIM1_CC_IRQn, //TIM1 have special Capture/Compare irq
	[1] = TIM2_IRQn,
	[2] = TIM3_IRQn,
	[3] = TIM4_IRQn,
};

static const
u16 ic_dma_source[4] = {
	[0] = TIM_DMA_CC1,
	[1] = TIM_DMA_CC2,
	[2] = TIM_DMA_CC3,
	[3] = TIM_DMA_CC4,
};

static const
dma_chnum_t ic_numch2dma_ch[] = {
    [0] = DMA_TIM1_UP,
    [1] = DMA_TIM2_UP,
    [2] = DMA_TIM3_UP,
    [3] = DMA_TIM4_UP,

	[0] = DMA_TIM1_CH1,
    [1] = DMA_TIM1_CH2,
    [2] = DMA_TIM1_CH3,
    [3] = DMA_TIM1_CH4,

	[4] = DMA_TIM2_CH1,
    [5] = DMA_TIM2_CH2,
    [6] = DMA_TIM2_CH3,
    [7] = DMA_TIM2_CH4,

    [8] = DMA_TIM3_CH1,
    [9] = DMA_TIM3_UP,//dma have not channel for TIM3_CH2
    [10] = DMA_TIM3_CH3,
    [11] = DMA_TIM3_CH4,

    [12] = DMA_TIM4_CH1,
    [13] = DMA_TIM4_CH2,
    [14] = DMA_TIM4_CH3,
    [15] = DMA_TIM4_UP,//dma have not channel for TIM4_CH4
};
