#define NET_MODULE
#define NET_LEGACY_STATE_TRACKER
#include "tracker_i.h"

#ifdef MAC_GENERATOR
/**
 * handle test state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg   
 */
void netlst_mac_gen(net_legacy_entry_t * restrict net,
                   const u8 com,
                   const u32 arg)
{
	/* todo: state should be done */
	switch(com){
		case NET_NOOP:
		{
			/* wait for new commands and scan with some timeout */
			if(0 < net->conn_limit){
				const b8 need_attention = !netl_com_twait(net, NETL_RX_PERIOD_US);
				if(likely(true == need_attention)){
					/* rx data if we do not get commands for a 150msec */
					net_legacy_data_t rxdata;

					const b8 rx_done = net->be->ft.receive(net->be, &rxdata);
					if(true == rx_done){
						/* chech the rx data */
						__attribute__((unused))
							const u64 mac = rxdata.raw;
						/* we got message from paired device */
						b8 need_emit = false;


						/* todo: 
						 * 
						 * what we should do in mac gen
						 */


						if(need_emit){
							inevent_event_arg4(net->input, EVENT_NET_LEGACY_INFO, 4);
						}
						/* in all cases do not recceive 2 seconds
						 * after recceiving the message
						 */
						msleep(3000);
					}
				}
			}else{
				/* wait for commands */
				netl_com_wait(net);
			}
		}
			break;

		case NET_STOP:
		{
			net->stat.state = NS_DISABLE;
			net->be->ft.off(net->be);
		}
			break;

		case NET_NORMAL:
		{
			net->stat.state = NS_NORMAL;
		}
			break;
	}
}
#endif