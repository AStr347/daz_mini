#define TRIGONOMETRY_MODULE
#include "trig_i.h"

/**
 * circle diagramm for s32 angle
 * angle deg	- value
 * 0		- 0
 * 90		- 0x4000
 * 180		- 0x8000
 * 270		- 0xC000 
 * 360		- 0x10000
 */
enum angles{
	__angle0 = 0,
	__angle90 = 0x4000,
	__angle180 = 0x8000,
};


/**
 * return values for trig, one num is forbidden
 * trig		- value
 * -1		- (-0x7fff)
 * 0		- 0
 * 1 		- (+0x7fff)
 */

/* for sin, cos we can use only part of circle - 0 ... sligtly less then 90 */

/**
 * sin
 * @param angle 	- angle, period is 0xffff + 1 
 * @return 		- sin -0x7fff .. +0x7fff
 */
__attribute__((const))
s16 sin16(s32 angle)
{
	b8 is_positive = 0 == ((u32)angle >> 31);
	u32 raw = __builtin_abs(angle) & 0xffff;

	if(raw >= __angle180){
		raw -= __angle180;
		is_positive = !is_positive;
	}

	if(raw >= __angle90){
		raw = __angle180 - raw;
	}

	const s16 result = sin_table((raw + 3) >> 3);

	return is_positive ? result : -result;
}

/**
 * cos
 * @param angle 	- angle, period is 0xffff + 1 
 * @return 		- cos -0x7fff .. +0x7fff
 */
__attribute__((const))
s16 cos16(s32 angle)
{
	b8 is_positive = true;
	u32 raw = __builtin_abs(angle) & 0xffff;

	if(raw >= __angle180){
		raw -= __angle180;
		is_positive = !is_positive;
	}

	if(raw >= __angle90){
		raw -= __angle90;
		is_positive = !is_positive;
	}else{
		raw = __angle90 - raw;
	}

	const s16 result = sin_table((raw + 3) >> 3);

	return is_positive ? result : -result;
}
