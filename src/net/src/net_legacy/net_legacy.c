/**
 * net module with simplified 
 * functionality
 */
#define NET_MODULE
#define NETLAYER_LEGACY_MODULE		0x0000005a
#include "net_legacy_i.h"
#include "configurations.h"

static
b8 com_put(THIS, const u8 * data, const u8 size){
	mlock(this->com_mutex);
	const b8 res = sfifo8_put(&this->combuf, data, size);
	if(res){
		csignal(this->cond);
	}
	mulock(this->com_mutex);
	return res;
}

/**
 * turn net subsystem to powersave state
 * @param net		- [inout] netlayer instance
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_off(THIS)
{
	static const
	u8 dtw[] = {
		[0] = NET_STOP,
	};
	return com_put(this, dtw, sizeof(dtw));
}

/**
 * wake net subsystem from powersave state
 * @param net		- [inout] netlayer instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
static
b8 netl_on(THIS)
{
	static const
	u8 dtw[] = {
		[0] = NET_START,
	};
	return com_put(this, dtw, sizeof(dtw));
}

/**
 * stop pairing/disparing and go to normal state
 * @param net		- [inout] net instance
 * 
 * @return		- true if successm
 * 			  false else
 */
static
b8 netl_normal(THIS)
{
	static const
	u8 dtw[] = {
		[0] = NET_NORMAL,
	};
	return com_put(this, dtw, sizeof(dtw));
}

/**
 * make request for start radio test state
 * @param net		- [inout] network instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
static
b8 netl_test_start(THIS)
{
	/* net legacy not support it */
	return false;
}

/**
 * start paring, max paired count limited cnt 
 * @param net		- [inout] net instance
 * @param cnt		- [in] max paired count due session
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_pairing(THIS, const u32 cnt)
{
	static const
	u8 dtw[] = {
		[0] = NET_PAIRING_START,
	};
	return com_put(this, dtw, sizeof(dtw));
}

/**
 * start disparing, max dispaired count limited cnt
 * @param net		- [inout] net instance
 * @param cnt		- [in] max dispaired count due session
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_dispairing(THIS, const u32 cnt)
{
	/* net legacy not support it */
	return false;
}

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
			 net_rx_t * restrict message)
{
	b8 result = false;

	mlock(this->rx_mutex);
	const u32 cnt = this->rx_count;
	if(0 < cnt){
		/* make rx pos 0/1 when get message */
		const u8 rx_pos = this->rx_pos;
		this->rx_pos ^= 1;
		message->id = this->rxdata[rx_pos].id;
		/* for a while legacy network do not have payload more then 2 bytes */
		const uf8 dcnt =  this->rxdata[rx_pos].data.cnt;
		message->data.cnt = dcnt;
		for(uf8 sel = 0; dcnt > sel;  ++sel){
			message->data.data[sel] = this->rxdata[rx_pos].data.data[sel];
		}
		
		this->rx_count = cnt - 1;
		result = true;
	}
	mulock(this->rx_mutex);
	return result;
}

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
				const net_data_t * restrict const data)
{
	u8 dtw[1 + sizeof(u64) + sizeof(net_data_t)] = {
		[0] = NET_MSG_SEND,
	};
	u8 * dst = dtw + 1;
	memcpy(dst, &id, sizeof(u64));
	dst += sizeof(u64);
	memcpy(dst, data, sizeof(net_data_t));
	return com_put(this, dtw, sizeof(dtw));
}

/* discard reccived messages
 * @param net		- [inout] network to use
 * 
 * @return		- true if something flushed
 * 			  false else
 */
static
b8 netl_flush(THIS)
{
	b8 result = false;
	mlock(this->rx_mutex);
	if(0 < this->rx_count){
		net_rx_t * restrict srx = this->rxdata;
		for(uf sel = 0; NET_LEGACY_RX_MAX > sel; ++sel, ++srx){
			srx->id = 0;
		}
		this->rx_count = 0;
		result = true;
	}
	mulock(this->rx_mutex);
	return result;
}

/**
 * enter mac requst state
 * @param net		- [inout] network instance
 * 
 * @return 		- true if success,
 * 			  false else 
 */
static
b8 netl_mac_req_on(THIS)
{
	static const
	u8 dtw[] = {
		[0] = NET_MAC_REQ_START,
	};
	return com_put(this, dtw, sizeof(dtw));
}

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
				   const u64 start_id)
{
	static const
	u8 dtw[] = {
		[0] = NET_MAC_GEN_START,
	};
	return com_put(this, dtw, sizeof(dtw));
}

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
				const u64 new_id)
{
	mlock(this->com_mutex);
	mlock(this->paired_mutex);
	mlock(this->rx_mutex);
	this->net.id = new_id;
	mulock(this->com_mutex);
	mulock(this->paired_mutex);
	mulock(this->rx_mutex);

	this_flush();
	return true;
}


/**
 * set new behavior to net subsystem
 * @param net		- [inout] network
 * @param behavior	- [in] new behavior
 * 
 * @return 		- true if success,
 * 			  false else
 */
static
b8 netl_behavior_set(THIS, const net_behavior_t new_beh)
{
	const
	u8 dtw[] = {
		[0] = NET_BEHAVIOR_CHANGE,
		[1] = new_beh,
	};
	return com_put(this, dtw, sizeof(dtw));
}

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
				  const net_data_t * restrict const data)
{
	/* net legacy not support it */
	return false;
}

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
					 const net_data_t * restrict const data)
{
	/* net legacy not support it */
	return false;
}

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
				   const net_data_t * restrict const data)
{
	/* net_legacy bcast - tx self id */
	u8 dtw[1 + sizeof(u64) + sizeof(net_data_t)] = {
		[0] = NET_MSG_SEND,
	};
	u8 * dst = dtw + 1;
	memcpy(dst, &this->net.id, sizeof(u64));
	dst += sizeof(u64);
	memcpy(dst, data, sizeof(net_data_t));
	return com_put(this, dtw, sizeof(dtw));
}

/**
 * legacy networks routines
 * @net		- [inout] network instance
 */
static
void net_legacy_routine(THIS)
{
	const net_legacy_config_t * const conf = this->conf;
	this->ll = conf->be_init(conf->be, conf->be_conf);
	for(;;){
		mlock(this->com_mutex);
		/* no operation by default */
		u8 com = NET_NOOP;
		const uf8 read_count = sfifo8_get(&this->combuf, (u8*)&com, 1);
		if(1 != read_count){
			sfifo8_reset(&this->combuf);
		}
		mulock(this->com_mutex);
		netl_state_tracker[this->stat.state](this, com);
	}
}


/**
 * init net subsystem,
 * start at sleep state
 * @param netl		- legacy network
 * @param config	- network configuration
 * @return		- true on success,
 * 			  false else
 */
gnet_t* net_legacy_init(THIS,
						const net_legacy_config_t * const conf)
{
	memclr(this, sizeof(net_legacy_entry_t));
	this->net.nft = &netlegacy_ft;
	this->net.this = this;
	this->net.id = 0x0;
	const net_behavior_t beh = conf->behavior;
	this->net.beh = conf->behavior;
	this->net.type = NETLAYER_LEGACY_TYPE;

	this->conf = conf;
	this->input = conf->input;

	this->stat.txactive = (0 < (beh & NETB_TX));
	this->stat.rxactive = (0 < (beh & NETB_RX));
	this->stat.wait = false;
	this->stat.state = NS_DISABLE;

	sfifo8_init(&this->combuf, this->combuf_data, NET_LEGACY_COMMAND_BUFFER_SIZE);

	minit(this->com_mutex);
	minit(this->rx_mutex);
	minit(this->paired_mutex);
	cinit(this->cond);

	TaskCreate(this, net_legacy_routine, NET_LEGACY_STACK_SIZE, NET_LEGACY_PRIORITY);

	return &this->net;
}
