/**
 * s16 trigonometry
 * so all circle is U16_MAX
 */

#ifndef TRIG_H
#define TRIG_H
#include "ia_arch.h"

#define TRIG_PERIOD		((u32)(0xffff + 1))
#define TRIG_PERIOD_M		(0xffff)

/**
 * sin
 * @param angle 	- angle, period is (U16_MAX + 1) 
 * @return 		- sin S16_MIN .. S16_MAX
 */
__attribute__((const))
s16 sin16(s32 angle);

/**
 * cos
 * @param angle 	- angle from -2pi(s16_MIN) to +2pi(s16 max) - 1 
 * @return 		- sin S16_MIN .. S16_MAX
 */
__attribute__((const))
s16 cos16(s32 angle);

#endif