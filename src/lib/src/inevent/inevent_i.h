/**
 * event engine internal module
 */
#ifndef INEVENT_MODULE
#error should be included to inevent.c only
#endif 
#include "inevent.h"
#include "string.h"

/**
 * contain event and event argument
 */
typedef struct __packed {
	u8 slot_num:5;	/* slot number 0 ... 63 */
	u8 arg_cnt:3;	/* arg count 0 ... 3,
			 * 0 - 0arg,
			 * 1 - 1byte,
			 * 2 - 2byte
			 * 3 - 4byte
			 * 4 - 4byte arg0, 4byte arg1
			 */
	u8 arg[0];	/* optional argument */
}inevent_frame_t;

#define INEVENT_SLOT_POS	(0)
#define INEVENT_SLOT_MASK	(0x1f)
#define INEVENT_ARG_POS		(5)
#define INEVENT_ARG_MASK	(7)
