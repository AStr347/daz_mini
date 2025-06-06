#define NET_MODULE
#define NET_LEGACY_STATE_TRACKER
#include "tracker_i.h"

/**
 * handle disparing state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg 
 */
void netlst_disparing(THIS, const net_command_t com)
{
	/* todo: disparing state not done yet */
	switch(com){
		case NET_NOOP: {
			netl_com_wait(this);
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