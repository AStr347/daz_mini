#define NET_MODULE
#define NET_LEGACY_STATE_TRACKER
#include "tracker_i.h"

/**
 * legacy network routine primitive,
 * don't check that id is connected
 * @param net			- [inout] legacy network
 * @param on_rx_signal		- [in] signal, that will be emit on recceive
 */
static
void netl_test_proceed(THIS)
{
	const b8 is_rx = this->stat.rxactive;
	if(false == is_rx){
		(void)netl_com_wait(this);
		return;
	}

	/* wait for new commands and scan with some timeout */
	const u32 wait_time_us = (80 + (random32() & 0x0f)) * 1000;
	const b8 need_attention = netl_com_twait(this, wait_time_us);
	if(need_attention){
		return;
	}
	/* rx data if we do not get commands for a 150msec */
	net_rx_t rxdata_buff[4];

	const u32 rx_cnt = this_ll_receive(rxdata_buff, 4);
	for(u8 sel = 0; rx_cnt > sel; ++sel){
		/* chech the rx data */
		const net_rx_t * const rxdata = rxdata_buff + sel;
		
		const u64 id = rxdata->id;
		const net_data_t * const data = &rxdata->data;

		mlock(this->paired_mutex);
		const u64 pid = netli_is_paired(id);
		mulock(this->paired_mutex);

		if(0 == pid){
			continue;
		}
		/* we got message from paired device */
		(void)netl_rx_put(this, pid, data);
		inevent_event(this->input, EVENT_NETL_INFO);
		msleep(150);
	}
	msleep(2500);
}

/**
 * handle test state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg   
 */
void netlst_test(THIS, const u8 com)
{
	switch (com){
		case NET_NOOP: {
			netl_test_proceed(this);
		} break;

		case NET_STOP: {
			this->stat.state = NS_DISABLE;
			this_ll_off();
		} break;

		case NET_NORMAL: {
			this->stat.state = NS_NORMAL;
		} break;

		case NET_BEHAVIOR_CHANGE: {
			net_behavior_t new_beh = NETB_UNDEF;
			const u8 readed = sfifo8_get(&this->combuf, &new_beh, 1);
			if(1 != readed){
				sfifo8_reset(&this->combuf);
				return;
			}
			netl_beh_set(this, new_beh);
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
			 * do nothing on
			 * NET_START
			 * NET_PAIRING_START
			 * NET_DISPAIRING_START
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
