#ifndef EXTINT_H
#define EXTINT_H
#include "ia_arch.h"
#include "dport.h"

typedef enum __packed {
	EXTIG_0_BIT = (1 << 0),
	EXTIG_1_BIT = (1 << 1),
	EXTIG_2_BIT = (1 << 2),
	EXTIG_3_BIT = (1 << 3),
	EXTIG_4_BIT = (1 << 4),
	EXTIG_5_BIT = (1 << 5),
	EXTIG_6_BIT = (1 << 6),
	EXTIG_7_BIT = (1 << 7),
	EXTIG_8_BIT = (1 << 8),
	EXTIG_9_BIT = (1 << 9),
	EXTIG_10_BIT = (1 << 10),
	EXTIG_11_BIT = (1 << 11),
	EXTIG_12_BIT = (1 << 12),
	EXTIG_13_BIT = (1 << 13),
	EXTIG_14_BIT = (1 << 14),
	EXTIG_15_BIT = (1 << 15),

	EXTIG_5_9_M = EXTIG_5_BIT | EXTIG_6_BIT | EXTIG_7_BIT | EXTIG_8_BIT | EXTIG_9_BIT,
	EXTIG_10_15_M = EXTIG_10_BIT | EXTIG_11_BIT | EXTIG_12_BIT | EXTIG_13_BIT | EXTIG_14_BIT | EXTIG_15_BIT,
} exti_group_t;

/**
 * init exti_ctx0 object
 */
extern
void extint_init(void);

/**
 * wait event from irq group
 * @param group - group index to wait
 * @param time - maximum timeout to wait
 * 
 * @return true - waited
 *         false - timeout
 */
extern
b8 extint_wait(const exti_group_t group,
               const u32 time);

/**
 * configure EXTI registers for port and pin IRQ
 * @param port - GPIO[A,B,C,D,E,F]
 * @param pin  - number of pin [0..15]
 * @param trig - trigger type [falling, rising, any] edge
 */
extern
exti_group_t extint_configure(const DPORT port,
                              const u8 pin,
                              const EXTITrigger_TypeDef trig);

#endif//EXTINT_H

#if defined(DEVICE_TREE_H)
/* nothing to extern */
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(EXTINT_DEVICE_TREE)
#define EXTINT_DEVICE_TREE
#define EXTINT_INIT()		    extint_init()
#endif
