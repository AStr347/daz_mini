#ifndef SPI_H
#define SPI_H
#include "ia_arch.h"
#include "dport.h"
#include "dma.h"

typedef SPI_TypeDef* SPI;

typedef enum {
    SPIM_FULL_DUPLEX,
    SPIM_ONLY_RX,
    SPIM_ONLY_TX,
    SPIM_CNT,
} spi_mode_t;

typedef struct {
    DPORT sdo_port;
    DPORT sdi_port;
    DPORT clk_port;

    u8 sdo_pin;
    u8 sdi_pin;
    u8 clk_pin;
    u8 num;
} spi_conf_t;

typedef struct {
    SPI hw;
    DPORT sdo_port;
    DPORT sdi_port;
    DPORT clk_port;

    u16 sdo_mask;
    u16 sdi_mask;
    u16 clk_mask;

    dma_chnum_t dma_ch_tx;
    dma_chnum_t dma_ch_rx;

    tdm_t mutex;
} spi_t;

static inline
void spi_lock(spi_t * const this){
    mlock(this->mutex);
}

static inline
void spi_unlock(spi_t * const this){
    mulock(this->mutex);
}

extern
void spi_init(spi_t * const this,
              const spi_conf_t * const conf);

extern
b8 spi_tx(spi_t * const this,
          const u8 * data,
          const u16 size);

extern
b8 spi_rx(spi_t * const this,
          u8 * data,
          const u16 size);

/**
 * reinit spi with new mode setting
 *
 * @param new_mode - [in] enum value for set new spi mode
 */
extern
void spi_change_mode(spi_t * const this, const spi_mode_t new_mode);

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
void spi_set_prescaler(spi_t * const this, const u16 new_prescaler);

#endif//SPI_H

#if defined(DEVICE_TREE_H)
#ifdef SPI1_HW
extern spi_t spi1;
#endif
#ifdef SPI2_HW
extern spi_t spi2;
#endif
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(SPI_DEVICE_TREE)
#define SPI_DEVICE_TREE

#ifdef SPI1_HW
spi_t spi1;
static const
spi_conf_t spi1_conf = {
    .sdo_port = SPI1_HW_SDO_PORT,
    .sdi_port = SPI1_HW_SDI_PORT,
    .clk_port = SPI1_HW_CLK_PORT,

    .sdo_pin = SPI1_HW_SDO_PIN,
    .sdi_pin = SPI1_HW_SDI_PIN,
    .clk_pin = SPI1_HW_CLK_PIN,
    .num = 1,
};
#define SPI1_INIT()		    spi_init(&spi1, &spi1_conf);
#else
#define SPI1_INIT()
#endif

#ifdef SPI2_HW
spi_t spi2;
static const
spi_conf_t spi2_conf = {
    .sdo_port = SPI2_HW_SDO_PORT,
    .sdi_port = SPI2_HW_SDI_PORT,
    .clk_port = SPI2_HW_CLK_PORT,

    .sdo_pin = SPI2_HW_SDO_PIN,
    .sdi_pin = SPI2_HW_SDI_PIN,
    .clk_pin = SPI2_HW_CLK_PIN,
    .num = 2,
};
#define SPI2_INIT()		    spi_init(&spi2, &spi2_conf);
#else
#define SPI2_INIT()
#endif

#endif
