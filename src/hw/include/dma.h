#ifndef DMA_H
#define DMA_H
#include "ia_arch.h"

#define DMA_CH_STEP         (sizeof(DMACH))
#define DMA_IRQCH_STEP      (4)

typedef DMA_Channel_TypeDef* DMACH;
typedef DMA_TypeDef* DMA;

/***
 * @brief dma channel number
 */
typedef enum __packed {
	DMA_CH1 = 0,
    DMA_ADC = DMA_CH1,
    DMA_TIM2_CH3 = DMA_CH1,
    DMA_TIM4_CH1 = DMA_CH1,

    DMA_CH2 = 1,
    DMA_SPI1_RX = DMA_CH2,
    DMA_USART3_TX = DMA_CH2,
    DMA_TIM1_CH1 = DMA_CH2,
    DMA_TIM2_UP = DMA_CH2,
    DMA_TIM3_CH3 = DMA_CH2,

    DMA_CH3 = 2,
    DMA_DAC_CH1 = DMA_CH3,
    DMA_SPI1_TX = DMA_CH3,
    DMA_USART3_RX = DMA_CH3,
    DMA_TIM1_CH2 = DMA_CH3,
    DMA_TIM3_CH4 = DMA_CH3,
    DMA_TIM3_UP = DMA_CH3,

    DMA_CH4 = 3,
    DMA_DAC_CH2 = DMA_CH4,
    DMA_SPI2_RX = DMA_CH4,
    DMA_USART1_TX = DMA_CH4,
    DMA_TIM1_CH4 = DMA_CH4,
    DMA_TIM1_TRIG = DMA_CH4,
    DMA_TIM1_COM = DMA_CH4,
    DMA_TIM4_CH2 = DMA_CH4,
    DMA_I2C2_TX = DMA_CH4,

    DMA_CH5 = 4,
    DMA_SPI2_TX = DMA_CH5,
    DMA_USART1_RX = DMA_CH5,
    DMA_TIM1_UP = DMA_CH5,
    DMA_TIM2_CH1 = DMA_CH5,
    DMA_TIM4_CH3 = DMA_CH5,
    DMA_I2C2_RX = DMA_CH5,

    DMA_CH6 = 5,
    DMA_USART2_RX = DMA_CH6,
    DMA_TIM1_CH3 = DMA_CH6,
    DMA_TIM3_CH1 = DMA_CH6,
    DMA_TIM3_TRIG = DMA_CH6,
    DMA_I2C1_TX = DMA_CH6,

    DMA_CH7 = 6,
    DMA_USART2_TX = DMA_CH7,
    DMA_TIM2_CH2 = DMA_CH7,
    DMA_TIM2_CH4 = DMA_CH7,
    DMA_TIM4_UP = DMA_CH7,
    DMA_I2C1_RX = DMA_CH7,

	DMA_CH_CNT,
} dma_chnum_t;

typedef enum {
	DMA_EBITS_TC_M = (1 << 0),
	DMA_EBITS_HTC_M = (1 << 1),
	DMA_EBITS_ERR_M = (1 << 2),
	DMA_EBITS_ALL_M = DMA_EBITS_TC_M | DMA_EBITS_HTC_M | DMA_EBITS_ERR_M,

	DMA_EBITS_STEP = 3,
} dma_ebits_t;


/***
 * @brief dma interrupts
 */
typedef enum __packed {
    DMAINT_GLOB = (1 << 0), /* global dma irq */
	DMAINT_TC = (1 << 1),  /* transfer completed */
	DMAINT_HTC = (1 << 2), /* half transfer completed */
	DMAINT_ERR = (1 << 3), /* error occured */
	DMAINT_ALL = (DMAINT_GLOB | DMAINT_TC | DMAINT_HTC | DMAINT_ERR),	/* add dma interrupts */
} dma_int_t;

extern
void dma_init(void);

/**
 * enable/disable IRQ and IT bits for choosen channel
 */
extern
void dma_set_waiting(const dma_chnum_t channel, const dma_ebits_t wait_flag);

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
extern
dma_ebits_t dma_twait(const dma_chnum_t channel, const dma_ebits_t wait_flag, const u32 time);

/**
 * wait anu irq on dma channel
 * @param channel - [in] dma channel to use
 * @param wait_flag - [in] dma waiting irq bits
 * 
 */
extern
dma_ebits_t dma_wait(const dma_chnum_t channel, const dma_ebits_t wait_flag);

/***
 * @brief stop selected dma channel and clear all it's interrupt flags
 * @param channel		- [in] channel to stop
 *
 * @return CNTR - for dma_rx can be matter
 */
extern
u32 dma_stop(const dma_chnum_t channel);

/***
 * @brief set circullar mode with transfer data from peripheral to memory
 * 
 * @param channel - [in] dma channel to use
 * @param pa - [in] peripheral address
 * @param mem - [in] memory address
 * @param data_cnt - [in] count of data for rx
 * @param data_size - [in] sizeof data receive (1, 2, 4 bytes)
 */
extern
void dma_circ_p2mem(const dma_chnum_t channel,
                    void * const pa,
                    const void * const mem,
                    const u32 data_cnt,
                    const u8 data_size);

/***
 * @brief set circullar mode with transfer data from memory to peripheral
 * 
 * @param channel - [in] dma channel to use
 * @param pa - [in] peripheral address
 * @param mem - [in] memory address
 * @param data_cnt - [in] count of data for rx
 * @param data_size - [in] sizeof data transmit (1, 2, 4 bytes)
 */
extern
void dma_circ_mem2p(const dma_chnum_t channel,
                    void * const pa,
                    const void * const mem,
                    const u32 data_cnt,
                    const u8 data_size);

/***
 * @brief set mode with transfer data from peripheral to memory
 * 
 * @param channel - [in] dma channel to use
 * @param pa - [in] peripheral address
 * @param mem - [in] memory address
 * @param data_cnt - [in] count of data for rx
 * @param data_size - [in] sizeof data receive (1, 2, 4 bytes)
 */
extern
void dma_rx_p2mem(const dma_chnum_t channel,
                    void * const pa,
                    const void * const mem,
                    const u32 data_cnt,
                    const u8 data_size);

/***
 * @brief set mode with transfer data from memory to peripheral
 * 
 * @param channel - [in] dma channel to use
 * @param pa - [in] peripheral address
 * @param mem - [in] memory address
 * @param data_cnt - [in] count of data for rx
 * @param data_size - [in] sizeof data transmit (1, 2, 4 bytes)
 */
extern
void dma_tx_mem2p(const dma_chnum_t channel,
                    void * const pa,
                    const void * const mem,
                    const u32 data_cnt,
                    const u8 data_size);


typedef struct {
    tdc_t cond;
    tdm_t mutex[DMA_CH_CNT];
    u16 will_wait;
} dma_ctx_t;
extern
dma_ctx_t dma_ctx;

static inline
void dma_lock(const dma_chnum_t ch){
	mlock(dma_ctx.mutex[ch]);
}

static inline
void dma_ulock(const dma_chnum_t ch){
	mulock(dma_ctx.mutex[ch]);
}

#endif//DMA_H

#if defined(DEVICE_TREE_H)
/* nothing to extern */
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(DMA_DEVICE_TREE)
#define DMA_DEVICE_TREE
dma_ctx_t dma_ctx;
#define DMA_INIT()		dma_init()
#endif
