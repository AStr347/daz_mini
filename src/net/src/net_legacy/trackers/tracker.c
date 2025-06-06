#define NET_MODULE
#define NET_LEGACY_STATE_TRACKER
#include "tracker_i.h"
#include "string.h"

/**
 * wait new net event 
 * @param net		- [inout] network instance
 * 
 */
void netl_com_wait(THIS)
{
	/* wait phase */
	mlock(this->com_mutex);
	const u32 com_cnt = this->combuf.cnt;
	if(0 == com_cnt){
		this->stat.wait = true;
		__unused
		u16 waitres = 0;
		cmwait(this->cond, this->com_mutex, waitres);
		this->stat.wait = false;
	}
	mulock(this->com_mutex);
}

/**
 * timedwait new net event 
 * @param net		- [inout] network instance
 * @param time		- [in] time to wait in us, should not be zero
 * 
 * @return          - true if waked by command,
 *                    false if timeout occurs
 */
b8 netl_com_twait(THIS,
                 const u32 time)
{
	s32 retval = 0;
	/* wait phase */
	mlock(this->com_mutex);
	const u32 com_cnt = this->combuf.cnt;
	if(0 == com_cnt){
		this->stat.wait = true;
		cmtwait(this->cond, this->com_mutex, ms2tick(time / 1000), retval);
		this->stat.wait = false;
	}
	mulock(this->com_mutex);

	return (1 == retval);
}

/**
 * set network behavior
 * @param net	- [in] network
 * @param beh	- [in] behavior to set
 */
void netl_beh_set(THIS,
                  const net_behavior_t beh)
{
	/* load tracker, based on selected network behavior */
	this->net.beh = beh;
	this->stat.txactive = 0 < (beh & NETB_TX);
	this->stat.rxactive = 0 < (beh & NETB_RX);
}

/**
 * send raw data over legacy network
 * @param net		- [inout] legacy network
 * @param raw		- [in] raw data
 * @param to_send	- [in] send data
 */
void netl_raw_send(THIS,
                   const u64 raw,
                   const u32 to_send)
{
	const u16 msg_data = to_send & MSG_DATA_M;
	const u8 __cnt = to_send >> MSG_DATA_CNT_P; 
	const u8 msg_count = 2 > __cnt? __cnt:2;
	
    const b8 paired = 0 < netli_is_real_paired(raw);
    const b8 bcast = (raw == this->net.id);
    
	if(likely(paired || bcast)){
		const net_data_t txdata = {
			.cnt = msg_count,
			.data[0] = msg_data & 0xff,
			.data[1] = (msg_data >> 8) & 0xff
		};
		this_ll_transmit(raw, &txdata);
	}
}

/**
 * add current put max
 * @param net		- [inout] legacy network
 * @param id		- [in] descriptor
 * @param data		- [in] data
 * 
 * @return 		- true, if data is not overwritten
 */
b8 netl_rx_put(THIS,
               const u64 id,
               const net_data_t * const data)
{
	mlock(this->rx_mutex);
	const u32 rxcnt = this->rx_count;
	const b8 result = NET_LEGACY_RX_MAX > rxcnt;
	const u8 rx_pos = this->rx_pos ^ 1;
	
	this->rx_pos = rx_pos;
	this->rxdata[rx_pos].id = id;
	const uf8 cnt = data->cnt;

	this->rxdata[rx_pos].data.cnt = cnt;
	for(uf8 sel = 0; cnt > sel; ++sel){
		this->rxdata[rx_pos].data.data[sel] = data->data[sel];	
	}

	this->rx_count = result ? rxcnt + 1 : NET_LEGACY_RX_MAX;
	mulock(this->rx_mutex);

	return result;
}

u64 netli_is_real_paired(const u64 id_check)
{
	const u64 rid = (id_check & NETL_REAL_ID_M);
	const b8 present = conf_net_present(&config, rid);
	return present? (id_check & NETL_RAW_RET_M) : 0x0;
}

u64 netli_is_paired(const u64 id_check)
{
	const u64 rid = (id_check & NETL_RAW_INFO_M);
	const b8 present = conf_net_present(&config, rid);
	return present? (id_check & NETL_RAW_RET_M) : 0x0;
}

/**
 * contain all states
 */
const netlst_t netl_state_tracker[NS_COUNT] = {
    [NS_NORMAL] = netlst_normal,
	[NS_CONN] = netlst_paring,
	[NS_DISCONN] = netlst_disparing,
	[NS_DISABLE] = netlst_disable,
	[NS_MAC_GEN] = NULL,
	[NS_MAC_REQ] = netlst_mac_req,
	[NS_TEST] = netlst_test,
};

