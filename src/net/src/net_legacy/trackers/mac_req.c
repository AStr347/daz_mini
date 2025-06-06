#define NET_MODULE
#define NET_LEGACY_STATE_TRACKER
#include "tracker_i.h"

static
void test_pocced(THIS){
	const b8 is_rx = (0 < this->conn_limit);
	if(false == is_rx){
		(void)netl_com_wait(this);
		return;
	}

	/* wait for new commands and scan with some timeout */
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
		
	const u64 id = rxdata.id & 0x0000ffffffffffff;
	const u64 wait_id = (this->net.id & 0x0000ffffffffff00) | TRIT4ENUM(2222);

	if(id == wait_id){
		/* set mac address considering */
		this->net.id = id & 0xffffffffffffff00;
		--this->conn_limit;
		this_flush();
		inevent_event(this->input, EVENT_NETL_INFO);
		/**
		 * in all cases do not recceive 2 seconds
		 * after recceiving the message
		 */
		msleep(2000);
	}
	if(40 > this->delay){
		++this->delay;
		return;
	}
	this->delay = 0;
	/* make the request to testbox */
	const u64 mac_req = NETL_BASE_ID_INFO | (this->net.id & 0x0000ffffffffff00) | TRIT4ENUM(1221);
	/**
	 * here was netl_raw_send
	 * but we didn't paired with testbox
	 */
	const net_data_t txdata = {
		.cnt = 0,
		.data[0] = 0,
		.data[1] = 0,
	};
	this_ll_transmit(mac_req, &txdata);
}



/**
 * handle mac request state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg   
 */
void netlst_mac_req(THIS, const net_command_t com)
{
	switch(com){
		case NET_NOOP: {
			test_pocced(this);
		} break;

		case NET_STOP: {
			this->stat.state = NS_DISABLE;
			this_ll_off();
		} break;

		case NET_NORMAL: {
			this->stat.state = NS_NORMAL;
		} break;

		default: {
			/**
			 * do nothing on:
			 * NET_START
			 * NET_PAIRING_START
			 * NET_DISPAIRING_START
			 * NET_BEHAVIOR_CHANGE
			 * NET_MAC_GEN_START
			 * NET_MAC_REQ_START
			 * NET_TEST_START
			 * NET_MAC_CHANGE
			 * NET_MSG_SEND
			 * NET_MSG_CANCEL
			 * NET_REPEAT_SEND
			 */
		} break;
	}
}
