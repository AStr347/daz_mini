#define DMA_MODULE
#include "dma_i.h"

static
void dma_irq_handler(const dma_chnum_t ch){
	const b8 is_wait = (0 < (dma_ctx.will_wait & (1 << ch)));
	const u32 mask = DMA_IRQ_CLR_M(ch);
	volatile u32 flags = (DMA1->INTFR & mask);

	DMA_ClearFlag(mask);
	NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn + ch);

	if(false == is_wait){
		return;
	}

	const u32 ebits = ((flags >> ((ch * DMA_IRQCH_STEP) + 1)) << (ch * DMA_EBITS_STEP));

	BaseType_t bogus = 0;
    xEventGroupSetBitsFromISR(dma_ctx.cond.cond, ebits, &bogus);
}

__attribute__((interrupt()))
void DMA1_Channel1_IRQHandler(void){ dma_irq_handler(DMA_CH1); }
__attribute__((interrupt()))
void DMA1_Channel2_IRQHandler(void){ dma_irq_handler(DMA_CH2); }
__attribute__((interrupt()))
void DMA1_Channel3_IRQHandler(void){ dma_irq_handler(DMA_CH3); }
__attribute__((interrupt()))
void DMA1_Channel4_IRQHandler(void){ dma_irq_handler(DMA_CH4); }
__attribute__((interrupt()))
void DMA1_Channel5_IRQHandler(void){ dma_irq_handler(DMA_CH5); }
__attribute__((interrupt()))
void DMA1_Channel6_IRQHandler(void){ dma_irq_handler(DMA_CH6); }
__attribute__((interrupt()))
void DMA1_Channel7_IRQHandler(void){ dma_irq_handler(DMA_CH7); }

static inline
void dma_clr(const dma_chnum_t channel)
{
	const u32 bits = DMAC_EBITS_ALL(channel);
	xEventGroupClearBits(dma_ctx.cond.cond, bits);
}

static inline
void dma_set_nvic(const dma_chnum_t channel, const b8 en)
{
	const NVIC_InitTypeDef dma_nvic = {
		.NVIC_IRQChannel = DMA1_Channel1_IRQn + channel,
		.Priority = 1,//0xe1,/* (1 << 4),*/
		.NVIC_IRQChannelCmd = en,
	};
	NVIC_Init(&dma_nvic);
}

void dma_init(void)
{
	memclr(&dma_ctx, sizeof(dma_ctx_t));
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    cinit(dma_ctx.cond);

    for(u8 i = DMA_CH1; i <= DMA_CH7; i++){
    	minit(dma_ctx.mutex[i]);
    	dma_stop(i);
    	dma_set_nvic(i, ENABLE);
	}
}

/**
 * enable/disable IRQ and IT bits for choosen channel
 */
void dma_set_waiting(const dma_chnum_t channel, const dma_ebits_t wait_flag)
{
	DMACH const dmach = dma_chs[channel];
	const u16 will_wait_mask = (1 << channel);

	dma_clr(channel);
	if(0 == wait_flag){
		dma_ctx.will_wait &= will_wait_mask;
		DMA_ITConfig(dmach, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE, DISABLE);
		dma_set_nvic(channel, DISABLE);
	} else {
		dma_ctx.will_wait |= will_wait_mask;
		DMA_ITConfig(dmach, (wait_flag << 1), ENABLE);
		dma_set_nvic(channel, ENABLE);
	}
}

/**
 * wait anu irq on dma channel or timeout
 * @param channel - [in] dma channel to use
 * @param wait_flag - [in] dma waiting irq bits
 * @param time - [in] dma waiting timeout in ticks
 * 					  portMAX_DELAY - not limited
 * 
 * @return 0 - timeout
 * 		   bit - if waited
 */
dma_ebits_t dma_twait(const dma_chnum_t channel, const dma_ebits_t wait_flag, const u32 time)
{
	const u32 bits_pos = (channel * DMA_EBITS_STEP);
	const u32 waiting = (wait_flag << bits_pos);
	volatile u32 waited = xEventGroupWaitBits(dma_ctx.cond.cond,
								 	 	 	  waiting,
											  pdTRUE,
											  pdFALSE,
											  time);

	const dma_ebits_t result = ((waited >> bits_pos) & DMA_EBITS_ALL_M);
    return result;
}

/**
 * wait anu irq on dma channel
 * @param channel - [in] dma channel to use
 * @param wait_flag - [in] dma waiting irq bits
 * 
 */
dma_ebits_t dma_wait(const dma_chnum_t channel, const dma_ebits_t wait_flag)
{
	return dma_twait(channel, wait_flag, portMAX_DELAY);
}

/***
 * @brief stop selected dma channel and clear all it's interrupt flags
 * @param channel		- [in] channel to stop
 *
 * @return CNTR - for dma_rx can be matter
 */
u32 dma_stop(const dma_chnum_t channel)
{
	dma_ctx.will_wait &= (1 << channel);
    DMACH const dmach = dma_chs[channel];
    const u32 result = dmach->CNTR;

    DMA_Cmd(dmach, DISABLE);
    DMA_DeInit(dmach);

    return result;
}

/***
 * @brief set circullar mode with transfer data from peripheral to memory
 * 
 * @param channel - [in] dma channel to use
 * @param pa - [in] peripheral address
 * @param mem - [in] memory address
 * @param data_cnt - [in] count of data for rx
 * @param data_size - [in] sizeof data receive (1, 2, 4 bytes)
 */
void dma_circ_p2mem(const dma_chnum_t channel,
                    void * const pa,
                    const void * const mem,
                    const u32 data_cnt,
                    const u8 data_size)
{
	DMACH const dmach = dma_chs[channel];
	const u16 pmem_bits = dma_pdata_size_flag[data_size];
	const u16 mmem_bits = dma_mdata_size_flag[data_size];

	const DMA_InitTypeDef DMA_InitStructure = {
			.DMA_PeripheralBaseAddr = (u32)pa,
			.DMA_MemoryBaseAddr = (u32)mem,
			.DMA_DIR = DMA_DIR_PeripheralSRC,
			.DMA_BufferSize = data_cnt,
			.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
			.DMA_MemoryInc = DMA_MemoryInc_Enable,
			.DMA_PeripheralDataSize = pmem_bits,
			.DMA_MemoryDataSize = mmem_bits,
			.DMA_Mode = DMA_Mode_Circular,
			.DMA_Priority = DMA_Priority_VeryHigh,
			.DMA_M2M = DMA_M2M_Disable,
	};

	DMA_Init(dmach, &DMA_InitStructure);
	DMA_Cmd(dmach, ENABLE);
}

/***
 * @brief set circullar mode with transfer data from memory to peripheral
 * 
 * @param channel - [in] dma channel to use
 * @param pa - [in] peripheral address
 * @param mem - [in] memory address
 * @param data_cnt - [in] count of data for rx
 * @param data_size - [in] sizeof data transmit (1, 2, 4 bytes)
 */
void dma_circ_mem2p(const dma_chnum_t channel,
                    void * const pa,
                    const void * const mem,
                    const u32 data_cnt,
                    const u8 data_size)
{
	DMACH const dmach = dma_chs[channel];
	const u16 pmem_bits = dma_pdata_size_flag[data_size];
	const u16 mmem_bits = dma_mdata_size_flag[data_size];

	const DMA_InitTypeDef DMA_InitStructure = {
			.DMA_PeripheralBaseAddr = (u32)pa,
			.DMA_MemoryBaseAddr = (u32)mem,
			.DMA_DIR = DMA_DIR_PeripheralDST,
			.DMA_BufferSize = data_cnt,
			.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
			.DMA_MemoryInc = DMA_MemoryInc_Enable,
			.DMA_PeripheralDataSize = pmem_bits,
			.DMA_MemoryDataSize = mmem_bits,
			.DMA_Mode = DMA_Mode_Circular,
			.DMA_Priority = DMA_Priority_VeryHigh,
			.DMA_M2M = DMA_M2M_Disable,
	};

	DMA_Init(dmach, &DMA_InitStructure);
	DMA_Cmd(dmach, ENABLE);
}

/***
 * @brief set mode with transfer data from peripheral to memory
 * 
 * @param channel - [in] dma channel to use
 * @param pa - [in] peripheral address
 * @param mem - [in] memory address
 * @param data_cnt - [in] count of data for rx
 * @param data_size - [in] sizeof data receive (1, 2, 4 bytes)
 */
void dma_rx_p2mem(const dma_chnum_t channel,
                    void * const pa,
                    const void * const mem,
                    const u32 data_cnt,
                    const u8 data_size)
{
	DMACH const dmach = dma_chs[channel];
	const u16 pmem_bits = dma_pdata_size_flag[data_size];
	const u16 mmem_bits = dma_mdata_size_flag[data_size];

	const DMA_InitTypeDef DMA_InitStructure = {
			.DMA_PeripheralBaseAddr = (u32)pa,
			.DMA_MemoryBaseAddr = (u32)mem,
			.DMA_DIR = DMA_DIR_PeripheralSRC,
			.DMA_BufferSize = data_cnt,
			.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
			.DMA_MemoryInc = DMA_MemoryInc_Enable,
			.DMA_PeripheralDataSize = pmem_bits,
			.DMA_MemoryDataSize = mmem_bits,
			.DMA_Mode = DMA_Mode_Normal,
			.DMA_Priority = DMA_Priority_VeryHigh,
			.DMA_M2M = DMA_M2M_Disable,
	};

	DMA_Init(dmach, &DMA_InitStructure);
	DMA_Cmd(dmach, ENABLE);
}

/***
 * @brief set mode with transfer data from memory to peripheral
 * 
 * @param channel - [in] dma channel to use
 * @param pa - [in] peripheral address
 * @param mem - [in] memory address
 * @param data_cnt - [in] count of data for rx
 * @param data_size - [in] sizeof data transmit (1, 2, 4 bytes)
 */
void dma_tx_mem2p(const dma_chnum_t channel,
                    void * const pa,
                    const void * const mem,
                    const u32 data_cnt,
                    const u8 data_size)
{
	DMACH const dmach = dma_chs[channel];
	const u16 pmem_bits = dma_pdata_size_flag[data_size];
	const u16 mmem_bits = dma_mdata_size_flag[data_size];

	const DMA_InitTypeDef DMA_InitStructure = {
			.DMA_PeripheralBaseAddr = (u32)pa,
			.DMA_MemoryBaseAddr = (u32)mem,
			.DMA_DIR = DMA_DIR_PeripheralDST,
			.DMA_BufferSize = data_cnt,
			.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
			.DMA_MemoryInc = DMA_MemoryInc_Enable,
			.DMA_PeripheralDataSize = pmem_bits,
			.DMA_MemoryDataSize = mmem_bits,
			.DMA_Mode = DMA_Mode_Normal,
			.DMA_Priority = DMA_Priority_VeryHigh,
			.DMA_M2M = DMA_M2M_Disable,
	};

	DMA_Init(dmach, &DMA_InitStructure);
	DMA_Cmd(dmach, ENABLE);
}
