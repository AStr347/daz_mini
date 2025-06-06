#ifndef DMA_MODULE
#error "must be include just into dma.c"
#endif

#include "dma.h"

#define DMAC_EBITS_ALL(__CH__)	(DMA_EBITS_ALL_M << (__CH__ * DMA_EBITS_STEP))

static const
u16 dma_pdata_size_flag[5] = {
    [0] = DMA_PeripheralDataSize_Byte,
    [1] = DMA_PeripheralDataSize_Byte,
    [2] = DMA_PeripheralDataSize_HalfWord,
    [3] = DMA_PeripheralDataSize_Word,
    [4] = DMA_PeripheralDataSize_Word,
};

static const
u16 dma_mdata_size_flag[5] = {
    [0] = DMA_MemoryDataSize_Byte,
    [1] = DMA_MemoryDataSize_Byte,
    [2] = DMA_MemoryDataSize_HalfWord,
    [3] = DMA_MemoryDataSize_Word,
    [4] = DMA_MemoryDataSize_Word,
};

static const
DMACH const dma_chs[] = {
	[DMA_CH1] = (DMACH) DMA1_Channel1_BASE,
	[DMA_CH2] = (DMACH) DMA1_Channel2_BASE,
	[DMA_CH3] = (DMACH) DMA1_Channel3_BASE,
	[DMA_CH4] = (DMACH) DMA1_Channel4_BASE,
	[DMA_CH5] = (DMACH) DMA1_Channel5_BASE,
	[DMA_CH6] = (DMACH) DMA1_Channel6_BASE,
	[DMA_CH7] = (DMACH) DMA1_Channel7_BASE,
};

#define DMA_IRQ_CLR_M(__CH__)   (0xf << (__CH__ * DMA_IRQCH_STEP))

