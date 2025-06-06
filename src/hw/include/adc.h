#ifndef ADC_H
#define ADC_H

#include "ia_arch.h"
#include "dport.h"
#include "board.h"

#define ADC_VREF_VOLT 	(125)

/**
 * init ADC1 and instance for work
 */
extern
void adc_init(void);

/**
 * get adc value for 1 ADC_Channel
 */
extern
u16 adc_get(const u8 ch);

/**
 * get volt value for 1 ADC_Channel
 */
extern
u16 adc_get_volt(const u8 ch);

/**
 * get temperature by ADC TempSensor
 */
extern
s32 adc_get_temp(void);

#endif//ADC_H

#if defined(DEVICE_TREE_H)
/* nothing to extern */
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(ADC_DEVICE_TREE)
#define ADC_DEVICE_TREE
#define ADC_INIT()		    adc_init()
#endif
