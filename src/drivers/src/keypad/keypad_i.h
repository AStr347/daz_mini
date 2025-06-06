#ifndef KEYPAD_MODULE
#error should be included to keypad module only
#endif
#include "keypad.h"

#define MOD		keypad_t * restrict const this

#define PROC_TIME_MS		(100)
#define KEYPAD_PRIORITY		(7)

#ifdef KEYPAD_DEBUG
#include "debug.h"
#define BUG(index)				bug(KEYPAD_MODULE, index)		
#define BUG_ON(cond, index)		bug_on(cond, KEYPAD_MODULE, index)
#else
#define BUG(a)
#define BUG_ON(a, b)
#endif

enum keypad_bugs {
	KEYPAD_BUG_INIT,
	KEYPAD_BUG_ROUTINE,
};
