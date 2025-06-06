#define SPI_MODULE
#include "spi_i.h"

#define SPI_BAUDRATE		(SPI_BaudRatePrescaler_256)
#define SPI_DMA_WATING		(DMA_EBITS_TC_M | DMA_EBITS_ERR_M)
#define SPI_WORK_TIME		(600)
#define SPI_RXTX_TIME		(600)

#define SPI_HARD_SLEEP(__CNT__)\
{\
	for(u32 i = 0; i < __CNT__; i++){\
		__asm__ volatile ("nop");\
	}\
}

void spi_init(THIS, const spi_conf_t * const conf)
{
    const u8 num = conf->num - 1;

    if(0 == num){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    }

	memclr(this, sizeof(spi_t));
    this->hw = spi_hws[num];
    this->sdo_port = conf->sdo_port;
    this->sdi_port = conf->sdi_port;
    this->clk_port = conf->clk_port;

    this->sdo_mask = (1 << conf->sdo_pin);
    this->sdi_mask = (1 << conf->sdi_pin);
    this->clk_mask = (1 << conf->clk_pin);

    this->dma_ch_rx = rx_chs[num];
    this->dma_ch_tx = tx_chs[num];
    minit(this->mutex);

    DPORT_AFOUT_INIT(this->clk_port, this->clk_mask);
    DPORT_AFOUT_INIT(this->sdo_port, this->sdo_mask);
    DPORT_AFIN_INIT(this->sdi_port, this->sdi_mask);
    
    SPI_Cmd(this->hw, DISABLE);

    SPI_I2S_ITConfig(this->hw, SPI_I2S_IT_TXE, DISABLE);
    SPI_I2S_ITConfig(this->hw, SPI_I2S_IT_RXNE, DISABLE);
	SPI_I2S_DMACmd(this->hw, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);

    /* default settings */
    static const
    SPI_InitTypeDef SPI_InitStruct = {
        .SPI_Direction = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode = SPI_Mode_Master,
        .SPI_DataSize = SPI_DataSize_8b,
        .SPI_CPOL = SPI_CPOL_Low,
        .SPI_CPHA = SPI_CPHA_1Edge,
        .SPI_NSS = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler = SPI_BAUDRATE,
        .SPI_FirstBit = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial = 0,
    };
    SPI_Init(this->hw, &SPI_InitStruct);
}

b8 spi_dma_tx(THIS,
              const u8 * data,
              const u16 size)
{
    dma_lock(this->dma_ch_tx);
	dma_set_waiting(this->dma_ch_tx, SPI_DMA_WATING);

    SPI_Cmd(this->hw, ENABLE);
    dma_tx_mem2p(this->dma_ch_tx,
                (void *)&this->hw->DATAR,
                data,
                size,
                1);

    const dma_ebits_t waited = dma_wait(this->dma_ch_tx, SPI_DMA_WATING);

	SPI_HARD_SLEEP(SPI_WORK_TIME);/* fix strange bug with lost last byte */
    SPI_Cmd(this->hw, DISABLE);

    dma_stop(this->dma_ch_rx);
    
	dma_set_waiting(this->dma_ch_tx, 0);
	dma_ulock(this->dma_ch_tx);
    return (DMA_EBITS_TC_M == waited);
}

b8 spi_reg_tx(THIS,
              const u8 * data,
              const u16 size)
{
	SPI_Cmd(this->hw, ENABLE);
    for(u16 i = 0; (i < size); ){
		const b8 txe = SPI_I2S_GetFlagStatus(this->hw, SPI_I2S_FLAG_TXE);
		if(txe){
			SPI_I2S_SendData(this->hw, data[i]);
			i++;
		}
		SPI_HARD_SLEEP(SPI_RXTX_TIME);/* fix strange bug with lost last byte */
	}
    SPI_HARD_SLEEP(SPI_WORK_TIME);/* fix strange bug with lost last byte */
    SPI_Cmd(this->hw, DISABLE);

    return true;
}

b8 spi_tx(THIS,
          const u8 * data,
          const u16 size)
{
   if(15 < size){
        return spi_dma_tx(this, data, size);
   } else {
       return spi_reg_tx(this, data, size);
   }
}

b8 spi_dma_rx(THIS,
              u8 * data,
              const u16 size)
{
    dma_lock(this->dma_ch_rx);
	dma_set_waiting(this->dma_ch_rx, SPI_DMA_WATING);
    SPI_Cmd(this->hw, ENABLE);

	dma_rx_p2mem(this->dma_ch_rx,
				 (void *)&this->hw->DATAR,
				 data,
				 size,
				 1);

	const dma_ebits_t waited = dma_wait(this->dma_ch_rx, SPI_DMA_WATING);

	SPI_HARD_SLEEP(SPI_WORK_TIME);/* fix strange bug with lost last byte */
	SPI_Cmd(this->hw, DISABLE);

	dma_stop(this->dma_ch_rx);

	dma_set_waiting(this->dma_ch_rx, 0);
    dma_ulock(this->dma_ch_rx);
	return (DMA_EBITS_TC_M == waited);
}

b8 spi_reg_rx(THIS,
              u8 * data,
              const u16 size)
{
	SPI_Cmd(this->hw, ENABLE);
	for(u16 i = 0; (i < size); ){
		const b8 rxne = SPI_I2S_GetFlagStatus(this->hw, SPI_I2S_FLAG_RXNE);
		if(rxne){
			data[i] = SPI_I2S_ReceiveData(this->hw);
			i++;
		}
		SPI_HARD_SLEEP(SPI_RXTX_TIME);/* fix strange bug with lost last byte */
	}
	SPI_HARD_SLEEP(SPI_WORK_TIME);/* fix strange bug with lost last byte */
    SPI_Cmd(this->hw, DISABLE);

    return true;
}

b8 spi_rx(THIS,
          u8 * data,
          const u16 size)
{
    if(15 < size){
        return spi_dma_rx(this, data, size);
    } else {
        return spi_reg_rx(this, data, size);
    }
}

/**
 * reinit spi with new mode setting
 * !!! not locking spi mutex !!!
 *
 * @param new_mode - [in] enum value for set new spi mode
 */
void spi_change_mode(THIS, const spi_mode_t new_mode){
    #define SPI_DIRECTION_CLR_M     (~SPI_Direction_1Line_Tx)

    static const
    u16 mode2direction[SPIM_CNT] = {
        [SPIM_FULL_DUPLEX] = SPI_Direction_2Lines_FullDuplex,
        [SPIM_ONLY_RX] = SPI_Direction_1Line_Rx,
        [SPIM_ONLY_TX] = SPI_Direction_1Line_Tx,
    };

    this->hw->CTLR1 &= SPI_DIRECTION_CLR_M;
    this->hw->CTLR1 |= mode2direction[new_mode];
}

/**
 * update SPIx Prescaler
 * 
 * @param this          - [inout] spi_t instance
 * @param new_prescaler - [in] values from std:
 *                          SPI_BaudRatePrescaler_2
 *                          SPI_BaudRatePrescaler_4
 *                          SPI_BaudRatePrescaler_8
 *                          SPI_BaudRatePrescaler_16
 *                          SPI_BaudRatePrescaler_32
 *                          SPI_BaudRatePrescaler_64
 *                          SPI_BaudRatePrescaler_128
 *                          SPI_BaudRatePrescaler_256
 */
void spi_set_prescaler(THIS, const u16 new_prescaler)
{
    mlock(this->mutex);

    this->hw->CTLR1 &= SPI_CTLR1_BR_CLR;
    this->hw->CTLR1 |= new_prescaler;

    mulock(this->mutex);
}
