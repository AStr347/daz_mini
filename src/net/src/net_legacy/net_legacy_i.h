#ifndef NETLAYER_LEGACY_MODULE
#error should be included only in net module
#endif
#include "net_legacy.h"
#include "trackers/tracker.h"
#include "string.h"
#include "net_legacy.h"

/* rx time for pulsar3 is extra time for analyse after we got correct preable
 * in test time, so we will rx if only in test time we got the preamble
 */
/*default lrec timeout in rx cycles*/
#define DEF_LREC_TIMEOUT		0


#define THIS net_legacy_entry_t * const this

/**
 * turn net subsystem to powersave state
 * @param net		- [inout] netlayer instance
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_off(THIS);

/**
 * wake net subsystem from powersave state
 * @param net		- [inout] netlayer instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
static
b8 netl_on(THIS);

/**
 * stop pairing/disparing and go to normal state
 * @param net		- [inout] net instance
 * 
 * @return		- true if successm
 * 			  false else
 */
static
b8 netl_normal(THIS);

/**
 * make request for start radio test state
 * @param net		- [inout] network instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
static
b8 netl_test_start(THIS);

/**
 * start paring, max paired count limited cnt 
 * @param net		- [inout] net instance
 * @param cnt		- [in] max paired count due session
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_pairing(THIS, const u32 cnt);

/**
 * start disparing, max dispaired count limited cnt
 * @param net		- [inout] net instance
 * @param cnt		- [in] max dispaired count due session
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_dispairing(THIS, const u32 cnt);

/**
 * read net message from net read buffer
 * @param net		- [inout] network to read
 * @param message	- [out] message
 * 
 * @return 		- true, if something was read
 * 			  false else
 */
static
b8 netl_read(THIS,
			 net_rx_t * restrict message);

/**
 * send message selected device 
 * 
 * @param net		- [inout] network
 * @param id		- [in] destination id
 * @param data		- [in] data to send
 * 
 * @return		- true if success,
 * 			  false else
 */
static
b8 netl_info_tx(THIS,
				const u64 id,
				const net_data_t * restrict const data);

/* discard reccived messages
 * @param net		- [inout] network to use
 * 
 * @return		- true if something flushed
 * 			  false else
 */
static
b8 netl_flush(THIS);

/**
 * enter mac requst state
 * @param net		- [inout] network instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
static
b8 netl_mac_req_on(THIS);

/**
 * enter mac generating state
 * @param net		- [inout] network instance
 * @param start_mac	- [in] start identificator
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_mac_gen_on(THIS,
				   const u64 start_id);

/**
 * set new mac address for current network
 * new mac should not be zero
 * @param net		- [inout] network
 * @param new_id	- [in] new identifier
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_mac_set(THIS,
				const u64 new_id);


/**
 * set new behavior to net subsystem
 * @param net		- [inout] network
 * @param behavior	- [in] new behavior
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_behavior_set(THIS, const u8 new_beh);

/**
 * send cancel message to selected device 
 * 
 * @param net		- [inout] network
 * @param id		- [in] destination id
 * @param data		- [in] data to send
 * 
 * @return		- true if success,
 * 			  false else
 */
static
b8 netl_cancel_tx(THIS,
				  const u64 id,
				  const net_data_t * restrict const data);

/**
 * send cancel message to all connected devices
 * 
 * @param net		- [inout] network
 * @param mac		- [in] destination
 * @param data		- [in] data to send
 * 
 * @return		- true if success,
 * 			  false else
 */
static
b8 netl_cancel_bcast(THIS,
					 const net_data_t * restrict const data);

/**
 * send message all connected devices
 * 
 * @param net		- [inout] network
 * @param mac		- [in] destination
 * @param data		- [in] data to send
 * 
 * @return		- true if success,
 * 			  false else
 */
static
b8 netl_info_bcast(THIS,
				   const net_data_t * restrict const data);


/* function table for puzzle pro network access */
static const
net_ft_t netlegacy_ft = {
	.off = (gnet_off_t) netl_off,
	.on = (gnet_on_t) netl_on,
	.normal = (gnet_normal_t) netl_normal,
	.test_start = (gnet_test_start_t) netl_test_start,

	.paring_on = (gnet_pairing_t) netl_pairing,
	.disparing_on = (gnet_dispairing_t) netl_dispairing,

	.read = (gnet_read_t) netl_read,
	.info_tx = (gnet_info_tx_t) netl_info_tx,
	.flush = (gnet_flush_t) netl_flush,

	.mac_req_on = (gnet_mac_req_on_t) netl_mac_req_on,
	.mac_gen_on = (gnet_mac_gen_on_t) netl_mac_gen_on,

	.mac_set = (gnet_mac_set_t) netl_mac_set,
	.beh_set = (gnet_behavior_set_t) netl_behavior_set,

	.cancel_tx = (gnet_cancel_tx_t) netl_cancel_tx,
	.cancel_bcast = (gnet_cancel_bcast_t) netl_cancel_bcast,
	.info_bcast = (gnet_info_bcast_t) netl_info_bcast,	
}; 
