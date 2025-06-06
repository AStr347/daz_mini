#ifndef MORZE_SIGNALS_H
#define MORZE_SIGNALS_H
#include "ia_arch.h"
#include "slice.h"

/**
 * container for danish morze melodies
 * @mem data		- data to play
 * @mem len		- length
 */
typedef struct msign_melody {
	const slice_t* restrict data;
	u16 len;
	u16 reserved;
} msign_melody_t;

/**
 * denmark alarm description
 * @enum TEST		- test signal
 * @enum BABY		- baby cry
 * @enum DBELL1		- door bell 1
 * @enum DBELL2		- door bell 2
 * @enum ICOM		- intercom
 * @enum PLLINE		- land line phone
 * @enum PCELL		- cell phone (mobile)
 * @enum SMOKE		- smoke alarm
 * @enum DWOPEN		- door/window open
 * @enum WATER		- water leek
 * @enum ASSIST		- need assistance
 * @enum CALARM		- alarm clock signal
 * @enum GAS		- leak of gas clone fire (TODO: create varios pattern)
 */
typedef enum __packed {
	ALD_FIRST = 0,
	ALD_TEST = 0,
	ALD_BABY,
	ALD_DBELL1,
	ALD_DBELL2,
	ALD_ICOM,
	ALD_PLLINE,
	ALD_PCELL,
	ALD_SMOKE,
	ALD_DWOPEN,
	ALD_WATER,
	ALD_ASSIST,
	ALD_GAS,
	ALD_CALARM,
	ALD_COMMON_COUNT,
	ALD_PWM_ON,
	ALD_PWM_OFF,
	ALD_PWM_PULS,
	ALD_PWM_CONNECT,
	ALD_COUNT,
} alarm_description_t;

#ifndef MORZE_SIGNALS_MODULE
/* Denmark morze melodies - mplay/lplay iface part */
extern const msign_melody_t msign_melodies[ALD_COMMON_COUNT];
#endif

#endif
