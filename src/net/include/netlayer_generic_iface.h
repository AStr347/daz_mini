/* interface for generic netlayer */
#ifndef NETLAYER_GENERIC_IFACE_H
#define NETLAYER_GENERIC_IFACE_H
#include "ia_arch.h"
#include "board.h"

/* network type definitions */
#ifndef NETLAYER_TYPE
#define NETLAYER_TYPE               0
#endif
#ifndef NETLAYER_LEGACY_TYPE
#define NETLAYER_LEGACY_TYPE		1
#endif

/**
 * @brief translate ID to net type
 */
#define ID_TO_TYPE(__ID__)          ((0 < ((__ID__)&((u64)0xffff000000000000))) ? NETLAYER_LEGACY_TYPE: NETLAYER_TYPE)

/* maximum length of recceived message */
#define NETDATA_LEN_MAX			16

/**
 * first byte of message payload is port,
 * for a while here will be legacy interface, where ports 0 ... 0x0E
 * reserved for legacy system purposes
 * 
 * @mem cancel		- indication cancel request 
 */
enum message_port {
	NET_NOT_A_MESSAGE = 0,
	NET_CONNECT_REQ,
	NET_CONNECT_ACK,
	NET_DISCONNECT_REQ,
	NET_DISCONNECT_ACK,
	NET_NACK,
	NET_CANCEL,	
	NET_INFO,
	NET_CONFIG,
	NET_REPEAT_CANCEL,
	NET_MAC_REQ,
	NET_MAC_ACK,
	NET_REPEAT,
	NET_REPEAT_REQ,
	NET_TEST,
	NET_MESSAGE_PORT_COUNT,
};

/**
 * @mem NETB_UNDEF		- behavior undefined
 * @mem NETB_TX			- device is transmitter
 * @mem NETB_RX			- device is recceiver
 * @mem NETB_TRX		- device is recceiver and transmitter
 * @mem NETB_GW			- device is gateway
 * @mem NETB_MSEEKER		- device is seeking for a new mac address
 * @mem NETB_MDEALER		- device is send mac addresses on request
 */
typedef enum __packed {
	NETB_UNDEF = 0,
	NETB_TX = (1 << 1),
	NETB_RX = (1 << 2),
	NETB_TRX = (NETB_TX | NETB_RX),
	NETB_GW = ((1 << 3) | NETB_TRX),
	NETB_MSEEKER = ((1 << 6) | NETB_TRX),
	NETB_MDEALER = ((1 << 7) | NETB_TRX),
} net_behavior_t;

/**
 * data format to pass through network ifaces
 * @mem cnt		- data count
 * @mem data		- payload
 */
typedef struct __packed net_data {
	u8 cnt;
	u8 data[NETDATA_LEN_MAX];
} net_data_t;

/**
 * network rx data
 * @mem id		- sender identifier address, if zero slot is empty
 * @mem data		- recceived data
 */
typedef struct __attribute__((packed, aligned(4))){
	u64 id;
	net_data_t data;
} net_rx_t;

/**
 * network information
 * @mam id		- network identifier
 * @mem beh		- network behavior
 * @mem type		- network type
 */
typedef struct {
	u64 id;
	u8 beh;
	u8 type;
} net_info_t;

/**
 * turn net subsystem to powersave state
 * @param net		- [inout] netlayer instance
 * 
 * @return 		- true if success,
 * 			  false else
 */
typedef b8 (*gnet_off_t)(void * const this);

/**
 * wake net subsystem from powersave state
 * @param net		- [inout] netlayer instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
typedef b8 (*gnet_on_t)(void * const this);

/**
 * stop pairing/disparing and go to normal state
 * @param net		- [inout] net instance
 * 
 * @return		- true if successm
 * 			  false else
 */
typedef b8 (*gnet_normal_t)(void * const this);

/**
 * make request for start radio test state
 * @param net		- [inout] network instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
typedef b8 (*gnet_test_start_t)(void * const this);

/**
 * start paring, max paired count limited cnt 
 * @param net		- [inout] net instance
 * @param cnt		- [in] max paired count due session
 * 
 * @return 		- true if success,
 * 			  false else
 */
typedef b8 (*gnet_pairing_t)(void * const this, const u32 cnt);

/**
 * start disparing, max dispaired count limited cnt
 * @param net		- [inout] net instance
 * @param cnt		- [in] max dispaired count due session
 * 
 * @return 		- true if success,
 * 			  false else
 */
typedef b8 (*gnet_dispairing_t)(void * const this, const u32 cnt);

/**
 * read net message from net read buffer
 * @param net		- [inout] network to read
 * @param message	- [out] message
 * 
 * @return 		- true, if something was read
 * 			  false else
 */
typedef b8 (*gnet_read_t)(void * const this,
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
typedef b8 (*gnet_info_tx_t)(void * const this,
							 const u64 id,
							 const net_data_t * restrict const data);

/* discard reccived messages
 * @param net		- [inout] network to use
 * 
 * @return		- true if something flushed
 * 			  false else
 */
typedef b8 (*gnet_flush_t)(void * const this);

/**
 * enter mac requst state
 * @param net		- [inout] network instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
typedef b8 (*gnet_mac_req_on_t)(void * const this);

/**
 * enter mac generating state
 * @param net		- [inout] network instance
 * @param start_mac	- [in] start identificator
 * 
 * @return 		- true if success,
 * 			  false else
 */
typedef b8 (*gnet_mac_gen_on_t)(void * const this,
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
typedef b8 (*gnet_mac_set_t)(void * const this,
							 const u64 new_id);


/**
 * set new behavior to net subsystem
 * @param net		- [inout] network
 * @param behavior	- [in] new behavior
 * 
 * @return 		- true if success,
 * 			  false else
 */
typedef b8 (*gnet_behavior_set_t)(void * const this, const net_behavior_t new_beh);

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
typedef b8 (*gnet_cancel_tx_t)(void * const this,
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
typedef b8 (*gnet_cancel_bcast_t)(void * const this,
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
typedef b8 (*gnet_info_bcast_t)(void * const this,
								const net_data_t * restrict const data);



/* IFACE FUNCTION TABLE */
typedef struct {
	gnet_off_t off;
	gnet_on_t on;
	gnet_normal_t normal;
	gnet_test_start_t test_start;
	
	gnet_pairing_t paring_on;
	gnet_dispairing_t disparing_on;
	
	gnet_read_t read;
	gnet_info_tx_t info_tx;
	gnet_flush_t flush;

	gnet_mac_req_on_t mac_req_on;
	gnet_mac_gen_on_t mac_gen_on;

	gnet_mac_set_t mac_set;
	gnet_behavior_set_t beh_set;
	
	gnet_cancel_tx_t cancel_tx;
	gnet_cancel_bcast_t cancel_bcast;
	gnet_info_bcast_t info_bcast;
} net_ft_t;

/* interface declarations */

/**
 * every network should have same interface to access to it's functions
 * @mem nft		- net function table
 * @mem id		- media access identifier
 * @mem rx_count	- count of rxdata, that present in net buffet
 * @mem type		- network type
 */
typedef struct gnet {
	const net_ft_t * nft;
	void * this;
	u64 id;
	net_behavior_t beh;
	u8 type;
} gnet_t;

typedef gnet_t * (*net_init)(void * const this, const void * const conf);

#ifdef NET_MODULE

#define this_off()                                      (this->net.nft->off(this))
#define this_on()                                       (this->net.nft->on(this))
#define this_normal()                                   (this->net.nft->normal(this))
#define this_test_start()                               (this->net.nft->test_start(this))

#define this_paring_on(__CNT__)                         (this->net.nft->paring_on(this, __CNT__))
#define this_disparing_on(__CNT__)                      (this->net.nft->disparing_on(this, __CNT__))

#define this_read(__MESSAGE__)                          (this->net.nft->read(this, __MESSAGE__))
#define this_info_tx(__ID__, __DATA__)                  (this->net.nft->info_tx(this, __ID__, __DATA__))
#define this_flush()                                    (this->net.nft->flush(this))

#define this_mac_req_on()                               (this->net.nft->mac_req_on(this))
#define this_mac_gen_on(__START__)                      (this->net.nft->mac_gen_on(this, __START__))

#define this_mac_set(__ID__)                            (this->net.nft->mac_set(this, __ID__))
#define this_beh_set(__BEH__)                           (this->net.nft->beh_set(this, __BEH__))

#define this_cancel_tx(__ID__, __DATA__)                (this->net.nft->cancel_tx(this, __ID__, __DATA__))
#define this_cancel_bcast(__DATA__)                     (this->net.nft->cancel_bcast(this, __DATA__))
#define this_info_bcast(__DATA__)                       (this->net.nft->info_bcast(this, __DATA__))

#endif

#endif
