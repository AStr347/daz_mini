#ifndef SPI_MODULE
#error "must be include just into spi.c"
#endif
#include "spi.h"

#define THIS spi_t * const this

static const
SPI const spi_hws[2] = {
    [0] = SPI1,
    [1] = SPI2,
};

static const
dma_chnum_t tx_chs[2] = {
    [0] = DMA_SPI1_TX,
    [1] = DMA_SPI2_TX,
};

static const
dma_chnum_t rx_chs[2] = {
    [0] = DMA_SPI1_RX,
    [1] = DMA_SPI2_RX,
};
