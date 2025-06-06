#ifndef SSIGN_MODULE
#error "must be include just into ssign.c"
#endif

#include "ssign.h"

#define SSIGN_PATTER_DEF(__NAME__, __CNT__)\
static const struct {\
    u8 len;\
    slice_t slice[(__CNT__)];\
} ssign_##__NAME__ = {\
    .len = (__CNT__),\
    .slice = {

#define LONG		(slice_t){.level = 1, .length = 500}
#define SHORT		(slice_t){.level = 1, .length = 125}
#define PAUSE		(slice_t){.level = 0, .length = 500}
#define DELIM		(slice_t){.level = 0, .length = 80}
#define FINAL		(slice_t){.level = 0, .length = 1000}

#define SSIGN_PATTER_END },};

/* test */
SSIGN_PATTER_DEF(test, 2)
	LONG,
	FINAL,
SSIGN_PATTER_END

/* baby cry */
SSIGN_PATTER_DEF(baby, 10)
	SHORT,
	DELIM,
	SHORT,
	DELIM,
	SHORT,
	DELIM,
	SHORT,
	DELIM,
	SHORT,
	FINAL,
SSIGN_PATTER_END

/* doorbell 1 */
SSIGN_PATTER_DEF(dbell1, 8)
	SHORT,
	DELIM,
	LONG,
	PAUSE,
	SHORT,
	DELIM,
	LONG,
	FINAL,
SSIGN_PATTER_END

/* doorbell 2 - at moment same as doorbell 1*/
SSIGN_PATTER_DEF(dbell2, 8)
	SHORT,
	DELIM,
	LONG,
	PAUSE,
	SHORT,
	DELIM,
	LONG,
	FINAL,
SSIGN_PATTER_END

/* intercom */
SSIGN_PATTER_DEF(icom, 8)
	SHORT,
	DELIM,
	SHORT,
	PAUSE,
	SHORT,
	DELIM,
	SHORT,
	FINAL,
SSIGN_PATTER_END

/* phone landline */
SSIGN_PATTER_DEF(plline, 6)
	LONG,
	PAUSE,
	LONG,
	PAUSE,
	LONG,
	FINAL,
SSIGN_PATTER_END

/* phone cell */
SSIGN_PATTER_DEF(pcell, 8)
	SHORT,
	PAUSE,
	SHORT,
	PAUSE,
	SHORT,
	PAUSE,
	SHORT,
	FINAL,
SSIGN_PATTER_END

/* smoke alarm */
SSIGN_PATTER_DEF(smoke, 14)
	LONG,
	DELIM,
	SHORT,
	DELIM,
	SHORT,
	DELIM,
	LONG,
	DELIM,
	SHORT,
	DELIM,
	SHORT,
	DELIM,	
	LONG,
	FINAL,
SSIGN_PATTER_END

/* door or window open */
SSIGN_PATTER_DEF(dwopen, 6)
	SHORT,
	FINAL,
	SHORT,
	FINAL,
	SHORT,
	FINAL,
SSIGN_PATTER_END

/* water leek */
SSIGN_PATTER_DEF(water, 9)
	LONG,
	DELIM,
	LONG,
	DELIM,
	FINAL,
	LONG,
	DELIM,
	LONG,
	FINAL,
SSIGN_PATTER_END

/* need assistance */
SSIGN_PATTER_DEF(assist, 6)
	LONG,
	DELIM,
	LONG,
	DELIM,
	LONG,
	FINAL,
SSIGN_PATTER_END
/* smoke alarm */
SSIGN_PATTER_DEF(gas, 14)
	LONG,
	DELIM,
	SHORT,
	DELIM,
	SHORT,
	DELIM,
	LONG,
	DELIM,
	SHORT,
	DELIM,
	SHORT,
	DELIM,
	LONG,
	FINAL,
SSIGN_PATTER_END


/* alarm clock signal */
SSIGN_PATTER_DEF(calarm, 12)
	LONG,
	PAUSE,
	LONG,
	PAUSE,
	LONG,
	PAUSE,
	SHORT,
	DELIM,
	SHORT,
	DELIM,
	SHORT,
	FINAL,	
SSIGN_PATTER_END

static const
ssign_pattern_t * const ssign_patterns[ALD_COUNT] = {
	[ALD_TEST] = (ssign_pattern_t *) & ssign_test,
	[ALD_BABY] = (ssign_pattern_t *) & ssign_baby,
	[ALD_DBELL1] = (ssign_pattern_t *) & ssign_dbell1,
	[ALD_DBELL2] = (ssign_pattern_t *) & ssign_dbell2,
	[ALD_ICOM] = (ssign_pattern_t *) & ssign_icom,
	[ALD_PLLINE] = (ssign_pattern_t *) & ssign_plline,
	[ALD_PCELL] = (ssign_pattern_t *) & ssign_pcell,
	[ALD_SMOKE] = (ssign_pattern_t *) & ssign_smoke,
	[ALD_DWOPEN] = (ssign_pattern_t *) & ssign_dwopen,
	[ALD_WATER] = (ssign_pattern_t *) & ssign_water,
	[ALD_ASSIST] = (ssign_pattern_t *) & ssign_assist,
	[ALD_GAS] = (ssign_pattern_t *) & ssign_gas,
	[ALD_CALARM] = (ssign_pattern_t *) & ssign_calarm,
	[ALD_COMMON_COUNT] = (ssign_pattern_t *) & ssign_test,
	[ALD_PWM_ON] = (ssign_pattern_t *) & ssign_test,
	[ALD_PWM_OFF] = (ssign_pattern_t *) & ssign_test,
	[ALD_PWM_PULS] = (ssign_pattern_t *) & ssign_test,
	[ALD_PWM_CONNECT] = (ssign_pattern_t *) & ssign_test,
};
