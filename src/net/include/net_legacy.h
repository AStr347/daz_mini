#ifndef NETWORK_LEGACY_H
#define NETWORK_LEGACY_H
#include "ia_arch.h"
#include "inevent.h"
#include "netlayer_generic_iface.h"
#include "net_legacy_be.h"
#include "link_legacy.h"
#include "sfifo8.h"
/* used for can be repeater macro */
#include "board.h"

/* base data about id len and timings */
#define NETL_BASE_ID_INFO    ((u64)0x1846 << 48)

#define REPEATER_NODES                      4
#define NET_LEGACY_COMMAND_BUFFER_SIZE		64
#define NET_LEGACY_COM_PAYLOAD_SIZE         32

#define NET_LEGACY_PRIORITY					(0)
#define NET_LEGACY_STACK_SIZE               (256)

#define NET_LEGACY_RX_MAX                   2
#ifndef NOTIFY_COUNT_MAX
#error max count of connected device must be defined
#endif
#define NET_LEGACY_CONN_MAX                 NOTIFY_COUNT_MAX

/**
 * net legacy message types for rx buffer
 * @enum NETLMT_INFO		- marks events that inform about new 
 * @enum NETLMT_CONN		- marks events that new connected req
 * 				  is emitted
 */
enum netl_type {
	NETLMT_INFO = 0,
	NETLMT_CONN = 1,
};


typedef enum __packed {
	/* one word commands */
	NET_NOOP = 0,
	NET_START,
	NET_STOP,
	NET_NORMAL,
	NET_PAIRING_START,
	NET_DISPAIRING_START,
	NET_BEHAVIOR_CHANGE,
	NET_MAC_GEN_START,
	NET_MAC_REQ_START,
	NET_TEST_START,
	/* long commands */
	NET_MSG_SEND,
	NET_MSG_CANCEL,	
	NET_REPEAT_SEND,
	NET_COMMAND_COUNT,
} net_command_t;

/**
 * legacy network modes
 * @enum SLEEP		- legacy network is sleeping
 * @enum NORMAL		- legacy network tx and rx data
 * @enum CONN		- legacy network is conneting
 * @enum DISCONN	- legacy network is disconnecting
 * @enum TEST		- legacy network test state
 */
typedef enum __packed {
	NS_DISABLE,
	NS_NORMAL,
	NS_CONN,
	NS_DISCONN,
	NS_MAC_GEN,
	NS_MAC_REQ,
	NS_TEST,
	NS_COUNT,
} netl_state_t;

/** config for network
 * @mem dev_type	- device type
 * @mem behavior	- device behavior
 */
typedef struct {
	ll_init_t be_init;
	void * const be;
    const void * const be_conf;
	inevent_t * input;
	net_behavior_t behavior;
} net_legacy_config_t;

/** legacy network workarea
 * @mem dev_type	- device type
 * @mem behavior	- device behavior
 * @mem tracker		- state tracker for current network
 */
typedef struct net_legacy_entry {
	gnet_t net;
	net_legacy_be_t* ll;
	inevent_t* restrict input;

	const net_legacy_config_t * conf;

	u8 delay;
	u8 conn_limit;
	u8 rx_count;
	u8 rx_pos;
	u8 timeout;

	struct {
		b8 txactive;
		b8 rxactive;
		b8 wait;
		netl_state_t state;
	} stat;

	/* rx data now have addr/data devision */
	net_rx_t rxdata[NET_LEGACY_RX_MAX];
	
	
	sfifo8_t combuf;
	u8 combuf_data[NET_LEGACY_COMMAND_BUFFER_SIZE];

	tdm_t com_mutex;
	tdm_t rx_mutex;
	tdm_t paired_mutex;
	tdc_t cond;
	TASK_T(NET_LEGACY_STACK_SIZE);
} net_legacy_entry_t;


/**
 * network state handler
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg
 */
typedef void(*netlst_t)(net_legacy_entry_t * const this,
						const net_command_t com);

/**
 * init net subsystem,
 * start at sleep state
 * @param netl		- legacy network
 * @param config	- network configuration
 * @return		- true on success,
 * 			  false else
 */
gnet_t* net_legacy_init(net_legacy_entry_t * const this,
						const net_legacy_config_t* conf);


/* legacy pulsar3 code */
#define DECDIV(a, n) (a%n)
#define CORRECTLEFT(a) ((a==1)?(3):(0))
#define CORRECT(a) ((a==2)?(1):(CORRECTLEFT(a)))

#define CORRECT_PACK_0(a) ((CORRECT(DECDIV(a,10UL))<<(6)))
#define CORRECT_PACK_1(a) ((CORRECT(DECDIV(a,100UL)/10L))<<(4))
#define CORRECT_PACK_2(a) ((CORRECT(DECDIV(a,1000UL)/100L))<<(2))
#define CORRECT_PACK_3(a) ((CORRECT(DECDIV(a,10000UL)/1000L))<<(0))


#define TRIT4ENUM(a) (CORRECT_PACK_3(a)<<6|CORRECT_PACK_2(a)<<2|CORRECT_PACK_1(a)>>2|CORRECT_PACK_0(a)>>6)
#define TRITGEN4(a) (CORRECT_PACK_3(a)|CORRECT_PACK_2(a)|CORRECT_PACK_1(a)|CORRECT_PACK_0(a))

#define TRITGEN(b,a) (TRITGEN4(b)|(TRITGEN4(a)<<8))

#endif /*NETWORK_LEGACY_H*/

#if defined(DEVICE_TREE_H)
extern net_legacy_entry_t netl0;
#ifndef net0
extern gnet_t * net0;
#else
extern gnet_t * net1;
#endif
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(NET_LEGACY_DEVICE_TREE)
#define NET_LEGACY_DEVICE_TREE

#if !defined(NET_LEGACY_BE_INIT) || !defined(NET_LEGACY_BE) || !defined(NET_LEGACY_BE_CONF)
#error "net legacy backend thing not defined"
#endif

#ifndef net0
gnet_t * net0;
#else
gnet_t * net1;
#endif

net_legacy_entry_t netl0;
static const
net_legacy_config_t net_legacy_conf0 = {
	.be_init = (ll_init_t)NET_LEGACY_BE_INIT,
	.be = NET_LEGACY_BE,
	.be_conf = NET_LEGACY_BE_CONF,
	.input = &INE_DEFAULT,
	.behavior = DEV_BEH,
};

#ifndef net0
#define NET_LEGACY_INIT()		net0 = net_legacy_init(&netl0, &net_legacy_conf0)
#else
#define NET_LEGACY_INIT()		net1 = net_legacy_init(&netl0, &net_legacy_conf0)
#endif


#endif
