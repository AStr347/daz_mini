#define NET_MODULE
#define NET_LEGACY_STATE_TRACKER
#include "tracker_i.h"
#include "log.h"

/**
 * legacy network routine primitive
 * @param net               - [inout] legacy network
 * @param on_rx_signal		- [in] signal, that will be emit on recceive
 */
static
void netl_proceed(THIS)
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

	b8 need_sleep = false;
	/* rx data if we do not get commands for a 150msec */
	net_rx_t rxdata_buff[4];

	const u32 rx_cnt = this_ll_receive(rxdata_buff, 4);
	for(u8 sel = 0; rx_cnt > sel; ++sel){
		/* chech the rx data */
		const net_rx_t * const rxdata = rxdata_buff + sel;
		
		const u64 id = rxdata->id;
		const net_data_t * const data = &rxdata->data;

		{
			/* strange but work, another ways break highest part */
			const u8 * const show = (u8*)&id;
			LOGW("rxdata->id: %02x%02x%02x%02x%02x%02x%02x%02x",\
					show[7], show[6], show[5], show[4],\
					show[3], show[2], show[1], show[0]);
		}

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
		need_sleep = true;
	}

	if(need_sleep){
		msleep(2500);
	}
}

/**
 * handle normal state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg
 */
void netlst_normal(THIS, const net_command_t com)
{
	switch(com){
		case NET_NOOP:{
			netl_proceed(this);
		}break;
		case NET_STOP:{
			this->stat.state = NS_DISABLE;
			this_ll_off();
		}break;
		case NET_PAIRING_START:{
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
		}break;
		case NET_BEHAVIOR_CHANGE:{
			net_behavior_t new_beh = NETB_UNDEF;
			const u8 readed = sfifo8_get(&this->combuf, &new_beh, 1);
			if(1 != readed){
				sfifo8_reset(&this->combuf);
				return;
			}
			netl_beh_set(this, new_beh);
		}break;
		case NET_MAC_REQ_START:{
			/* we will recceive new mac only one time */
			#warning "netl rf testing use random id, pulsar-3 should set fixed id back"
            this->net.id = ((random32() | 0x8000) & 0xffff00);
			this->conn_limit = 1;
			this->delay = 0;
			this->stat.state = NS_MAC_REQ;
			this_flush();
		}break;
		case NET_TEST_START:{
			this->stat.state = NS_TEST;
			this_flush();
		}break;
		case NET_MSG_SEND:{
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
		}break;

		default:{
			/**
			 * do nothing on
			 * NET_START
			 * NET_NORMAL
			 * NET_DISPAIRING_START
			 * NET_MAC_GEN_START
			 * NET_MSG_CANCEL
			 * NET_REPEAT_SEND
			 */
		} break;
	}
}
