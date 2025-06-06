#ifndef NETL_STATE_TRACKER_H
#define NETL_STATE_TRACKER_H
#include "net_legacy.h"
//#include "random.h"
//#include "string.h"

#define MSG_DATA_M	0xffff
#define MSG_DATA_CNT_P	16

/* used to skip the length/encoding and e.t.c params */
#define NETL_REAL_ID_M      ((u64)0xff00ffffffffffff)
#define NETL_RAW_INFO_M		NETL_REAL_ID_M
#define NETL_RAW_RET_M      ((u64)0xffffffffffffffff)

#define NETL_RX_PERIOD_MS	150
#define NETL_RX_PERIOD_US	150000

/**
 * contain all states
 */
extern const
netlst_t netl_state_tracker[NS_COUNT];

#endif
