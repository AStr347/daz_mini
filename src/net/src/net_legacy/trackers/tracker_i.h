#ifndef NET_LEGACY_STATE_TRACKER
#error must be include just into net legacy state trckers
#endif

#include "tracker.h"
#include "configurations.h"

/* used to simplify access to net legacy guts */
#define THIS net_legacy_entry_t * const this


/**
 * |||||||||||||||||||||||||||||||||||||||||||||||||
 * |||||||||||functions for trackers work|||||||||||
 * |||||||||||||||||||||||||||||||||||||||||||||||||
 */
/**
 * wait new net event
 * @param net		- [inout] network instance
 *
 */
extern
void netl_com_wait(THIS);

/**
 * timedwait new net event
 * @param net		- [inout] network instance
 * @param time		- [in] time to wait in us, should not be zero
 *
 * @return          - true if waked by command,
 *                    false if timeout occurs
 */
extern
b8 netl_com_twait(THIS,
                 const u32 time);

/**
 * set network behavior
 * @param net	- [in] network
 * @param beh	- [in] behavior to set
 */
extern
void netl_beh_set(THIS,
                  const net_behavior_t beh);

/**
 * send raw data over legacy network
 * @param net		- [inout] legacy network
 * @param raw		- [in] raw data
 * @param to_send	- [in] send data
 */
extern
void netl_raw_send(THIS,
                   const u64 raw,
                   const u32 to_send);

/**
 * add current put max
 * @param net		- [inout] legacy network
 * @param id		- [in] descriptor
 * @param data		- [in] data
 *
 * @return 		- true, if data is not overwritten
 */
extern
b8 netl_rx_put(THIS,
               const u64 id,
               const net_data_t * const data);

extern
u64 netli_is_real_paired(const u64 id_check);

extern
u64 netli_is_paired(const u64 id_check);

/**
 * |||||||||||||||||||||||||||||||||||||||||||||||||
 * |||||||||||||||trackers functions||||||||||||||||
 * |||||||||||||||||||||||||||||||||||||||||||||||||
 */

/**
 * empty tracker for disable state, do not proc any tasks
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg  
 */
extern
void netlst_disable(THIS, const net_command_t com);

/**
 * handle disparing state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg 
 */
extern
void netlst_disparing(THIS, const net_command_t com);

/**
 * handle mac request state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg   
 */
extern
void netlst_mac_req(THIS, const net_command_t com);


/**
 * handle normal state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg
 */
extern
void netlst_normal(THIS, const net_command_t com);

/**
 * handle test state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg   
 */
extern
void netlst_paring(THIS, const net_command_t com);

/**
 * handle test state
 * @param net		- [inout] network instance
 * @param com		- [in] command
 * @param arg		- [in] command arg   
 */
extern
void netlst_test(THIS, const net_command_t com);
