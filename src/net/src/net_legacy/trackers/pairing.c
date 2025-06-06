#define NET_MODULE
#define NET_LEGACY_STATE_TRACKER
#include "tracker_i.h"

static
void pairing_pocced(THIS){
	/* wait for new commands and scan with some timeout */
	if(0 == this->conn_limit){
		netl_com_wait(this);
		return;
	}
	const b8 need_attention = netl_com_twait(this, NETL_RX_PERIOD_US);
	if(need_attention){
		return;
	}
	/* rx data if we do not get commands for a 150msec */
	net_rx_t rxdata;
	const u32 rx_cnt = this_ll_receive(&rxdata, 1);
	if(0 == rx_cnt){
		return;
	}
	/* chech the rx data */
	u64 mac = rxdata.id;

	mlock(this->paired_mutex);
	const u64 pid = netli_is_real_paired(mac);
	if(0 == pid){
		--this->conn_limit;
	} else {
		mac = pid;
	}
	mulock(this->paired_mutex);
	const b8 puted = netl_rx_put(this, mac, &rxdata.data);
	if(false == puted){
		return;
	}
	inevent_event(this->input, EVENT_NETL_INFO);
	msleep(2000);
}

/**
 * handle test state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg   
 */
void netlst_paring(THIS, const net_command_t com)
{
	switch(com){
		case NET_NOOP: {
			pairing_pocced(this);
		} break;

		case NET_STOP: {
			this->stat.state = NS_DISABLE;
			this_ll_off();
		} break;

		case NET_NORMAL: {
			this->stat.state = NS_NORMAL;
		} break;
		
		case NET_PAIRING_START: {
			u8 conn_limit = 0;
			const u8 readed = sfifo8_get(&this->combuf, &conn_limit, 1);
			if(1 != readed){
				sfifo8_reset(&this->combuf);
				return;
			}
			this->stat.state = NS_CONN;
			this->conn_limit = conn_limit;
			/* flush recceived data */
			this->rx_count = 0;
		} break;
		
		case NET_MSG_SEND: {
			u64 target_id = 0;
			u8 readed = sfifo8_get(&this->combuf, (u8*)&target_id, sizeof(u64));
			if(sizeof(u64) != readed){
				sfifo8_reset(&this->combuf);
				return;
			}
			u32 to_send = 0;
			readed = sfifo8_get(&this->combuf, (u8*)&to_send, sizeof(u32));
			if(sizeof(u32) != readed){
				sfifo8_reset(&this->combuf);
				return;
			}
			netl_raw_send(this, target_id, to_send);
		} break;

		default: {
			/**
			 * do nothing on:
			 * NET_DISPAIRING_START
			 * NET_BEHAVIOR_CHANGE
			 * NET_MAC_GEN_START
			 * NET_MAC_REQ_START
			 * NET_TEST_START
			 * NET_MAC_CHANGE
			 * NET_MSG_CANCEL
			 * NET_REPEAT_SEND
			 */
		} break;
	}
}
