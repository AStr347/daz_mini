/* used to define simplified states */
#ifndef INEVENT_STATE
#define INEVENT_STATE
#include "inevent.h"

/* macro to define state with selected
 * @mem name	- state name
 * @mem count	- count of states
 */
#define INE_STATE(__name__, __count__)\
const struct __packed {\
	u8 count;\
	event_block_t block[__count__];\
} __name__ = 

/* jump to new stated */
#define STATE_JMP(__STATE__)	inevent_event_arg8(&INE_DEFAULT, EVENT_LIST, (u32)(&INE_DEFAULT), (u32)(__STATE__))

/**
 * 
 * some old macro thing for not reworkin states
 * 
 */

/* Wind timeout to force */
#define WIND_TIMEOUT	(2)
/* macro to SDATA buttons */
#define BUTTONS_STRUCTS  \
struct{\
    struct {\
        u8 st;\
        u8 time;\
    } left;\
    struct {\
        u8 st;\
        u8 time;\
    }right;\
    struct {\
        u8 st;\
        u8 time;\
    } escape;\
    struct {\
        u8 st;\
        u8 time;\
    } enter;\
}btn
    
/* prolog buttons sdata init */
#define PROLOG_BUTTONS_INIT \
SDATA->btn.right.st = BTN_R;\
SDATA->btn.right.time = 0;\
SDATA->btn.left.st = BTN_R;\
SDATA->btn.left.time = 0;\
SDATA->btn.enter.st = BTN_R;\
SDATA->btn.enter.time = 0;\
SDATA->btn.escape.st = BTN_R;\
SDATA->btn.escape.time = 0;

/* prolog buttons sdata init */
#define PROLOG_BUTTONS_INIT_U \
SDATA->btn.right.st = BTN_U;\
SDATA->btn.right.time = 0;\
SDATA->btn.left.st = BTN_U;\
SDATA->btn.left.time = 0;\
SDATA->btn.enter.st = BTN_U;\
SDATA->btn.enter.time = 0;\
SDATA->btn.escape.st = BTN_U;\
SDATA->btn.escape.time = 0;
	

/* increase & decrease functions */
#define _INCREASE static void state_inc(void)
#define _DECREASE static void state_dec(void)
/* draw function*/
#define _S_DRAW static void state_draw(void)
/* exit function*/
#define _S_EXIT static void state_exit(void)
#define _INC state_inc();
#define _DEC state_dec();
#define _DRAW state_draw();
#define _EXIT state_exit();


/* shoud be included to ON_RIGHT_P() */
#define _RIGHT_LONG_P  \
	SDATA->btn.right.time = 0;\
	SDATA->timeout = (STATE_DEFAULT_TIMEOUT << 1);\
	SDATA->btn.right.st = BTN_P;

/* shoud be included to ON_RIGHT_R() */
#define _RIGHT_LONG_R  \
	SDATA->btn.right.time = 0;\
	SDATA->timeout = (STATE_DEFAULT_TIMEOUT << 1);\
	SDATA->btn.right.st = BTN_R;

/* shoud be included to ON_LEFT_P() */
#define _LEFT_LONG_P  \
	SDATA->timeout = (STATE_DEFAULT_TIMEOUT << 1);\
	SDATA->btn.left.time = 0;\
	SDATA->btn.left.st = BTN_P;

/* shoud be included to ON_LEFT_R() */
#define _LEFT_LONG_R  \
	SDATA->timeout = (STATE_DEFAULT_TIMEOUT << 1);\
	SDATA->btn.left.time = 0;\
	SDATA->btn.left.st = BTN_R;
/*for pocket receiver */
#define _UP_LONG_P  \
	SDATA->btn.right.time = 0;\
	SDATA->timeout = (STATE_DEFAULT_TIMEOUT << 1);\
	SDATA->btn.right.st = BTN_P;

/* shoud be included to ON_RIGHT_R() */
#define _UP_LONG_R  \
	SDATA->btn.right.time = 0;\
	SDATA->timeout = (STATE_DEFAULT_TIMEOUT << 1);\
	SDATA->btn.right.st = BTN_R;

/* shoud be included to ON_LEFT_P() */
#define _DOWN_LONG_P  \
	SDATA->timeout = (STATE_DEFAULT_TIMEOUT << 1);\
	SDATA->btn.left.time = 0;\
	SDATA->btn.left.st = BTN_P;

/* shoud be included to ON_LEFT_R() */
#define _DOWN_LONG_R  \
	SDATA->timeout = (STATE_DEFAULT_TIMEOUT << 1);\
	SDATA->btn.left.time = 0;\
	SDATA->btn.left.st = BTN_R;     

/** macro generate long push and force forward digits*/
#define BUTTONS_FORCE_MACRO \
	if((SDATA->btn.right.st == BTN_P)&&(SDATA->btn.left.st != BTN_P)){\
		if(SDATA->btn.right.time < WIND_TIMEOUT){\
            ++SDATA->btn.right.time;\
		}else{\
			_INC\
		}\
	}else if((SDATA->btn.left.st == BTN_P)&&(SDATA->btn.right.st != BTN_P)){\
		if(SDATA->btn.left.time < WIND_TIMEOUT){\
			++SDATA->btn.left.time;\
		}else{\
			_DEC\
		}\
	}else{\
	SDATA->btn.right.time = 0;\
	SDATA->btn.left.time = 0;\
        /* proceed timeout */\
        if(likely(SDATA->timeout)){\
            --SDATA->timeout;\
        }else{\
            STATE_JMP(&main_menu_st);\
        }\
    }

/*Alls events for four buttons & HSEC*/
#define _ALL_FORCE_BUTTONS \
ON_HSEC()\
{\
    BUTTONS_FORCE_MACRO\
}\
ON_RIGHT_P()\
{\
    _RIGHT_LONG_P\
	_INC\
}\
ON_RIGHT_R()\
{\
    _RIGHT_LONG_R\
}\
ON_LEFT_P()\
{\
	_LEFT_LONG_P\
    _DEC\
}\
ON_LEFT_R()\
{\
    _LEFT_LONG_R\
}

#endif
