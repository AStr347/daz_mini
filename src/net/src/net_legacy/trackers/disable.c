#define NET_MODULE
#define NET_LEGACY_STATE_TRACKER
#include "tracker_i.h"

/**
 * empty tracker for disable state, do not proc any tasks
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg  
 */
void netlst_disable(THIS, const net_command_t com)
{
	/* from disable state we only wait start command,
	 * all other commands ignored
	 */
	switch (com){
		case NET_NOOP: {
			netl_com_wait(this);
		} break;

		case NET_START: {
			this->stat.state = NS_NORMAL;
			this_ll_on();
		} break;
		
		default: {
			/**
			 * do nothing on:
			 * NET_STOP
			 * NET_NORMAL
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