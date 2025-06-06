#ifndef LINK_LEGACY_MODULE
#error should be included to link_legac.c only
#endif
#include "link_legacy.h"
//#include "pulsar_any.h"
#include "pulsar_catcher.h"
//#include "kaku_any.h"
//#include "kaku_old.h"

/* link legacy module */
#define THIS         link_legacy_t * restrict const this

/**
 * legacy network make selector
 */
static const
legacy_make_t legacy_make[llenc_real_cnt] = {
	[llenc_puls] = (legacy_make_t)pulsc_make,
	[llenc_kaku] = (legacy_make_t)NULL,
	[llenc_kaku_old] = (legacy_make_t)NULL,
};

/**
 * legacy network parse selector
 */
static const
 legacy_parse_t legacy_parse[llenc_real_cnt] = {
	[llenc_puls] = (legacy_parse_t)pulsc_parse,
	[llenc_kaku] = (legacy_parse_t)NULL,
	[llenc_kaku_old] = (legacy_parse_t)NULL,
};

/**
 * legacy network test selector
 */
static const
legacy_test_t legacy_test[llenc_real_cnt] = {
	[llenc_puls] = (legacy_test_t)pulsc_test,
	[llenc_kaku] = (legacy_test_t)NULL,
	[llenc_kaku_old] = (legacy_test_t)NULL,
};


static
void ll_on(THIS);

static
void ll_off(THIS);

static
u32 ll_rx(THIS, net_rx_t* restrict const data, const u8 data_size);

static
b8 ll_tx(THIS, const u64 id, const net_data_t* restrict const data);

static const 
net_legacy_be_ft_t nft_common = {
    .on = (ll_on_t)ll_on,
	.off = (ll_off_t)ll_off,
	.receive = (ll_receive_t)ll_rx,
	.transmit = (ll_transmit_t)ll_tx,
};
