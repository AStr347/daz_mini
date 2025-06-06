#ifndef LINK_LEGACY_H
#define LINK_LEGACY_H
#include "ia_arch.h"
#include "link_legacy_be.h"
#include "net_legacy_be.h"

#define LINK_LEGACY_SLICE_COUNT_MAX		(256)

/**
 * link legacy configuration
 * @param init		- backend init func
 * @param be		- backend
 * @param be_conf	- backend_config
 */
typedef struct {
	ll_be_init_t be_init;
	void * const be;
	const void * be_conf;
	u32 freq;
}link_legacy_conf_t;

/**
 * link legacy instance
 * @mem ft		- link legacy function table
 */
typedef struct {
	net_legacy_be_t link;
	ll_be_t * be;
	u32 freq;
	struct {
		u32 cnt;
		slice_t data[LINK_LEGACY_SLICE_COUNT_MAX];
	} slices;
} link_legacy_t;

/**
 * init legacy linklayer
 * @param ll		- legacy linlayer instance
 * @param conf		- config
 */
extern
net_legacy_be_t* link_legacy_init(link_legacy_t * const this,
                      			  const link_legacy_conf_t * const conf);

#endif

#if defined(DEVICE_TREE_H)
extern link_legacy_t link_legacy0;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(LINK_LEGACY_DEVICE_TREE)
#define LINK_LEGACY_DEVICE_TREE

#if !defined(LINK_LEGACY_BE_INIT) || !defined(LINK_LEGACY_BE) || !defined(LINK_LEGACY_BE_CONF)
#error "link legacy backend thing not defined"
#endif

link_legacy_t link_legacy0;
static const
link_legacy_conf_t link_legacy_conf0 = {
	.be_init = (ll_be_init_t)LINK_LEGACY_BE_INIT,
	.be = LINK_LEGACY_BE,
	.be_conf = LINK_LEGACY_BE_CONF,
	.freq = LLINK_FREQ,
};

#define NET_LEGACY_BE_INIT		(link_legacy_init)
#define NET_LEGACY_BE			(&link_legacy0)
#define NET_LEGACY_BE_CONF		(&link_legacy_conf0)
#endif
