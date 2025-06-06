#define ADC_MODULE
#include "adc_i.h"
#include "log.h"

typedef struct {
    tdc_t waiter;
    tdm_t mutex;

    s16 calibval;
} adc_ctx_t;

adc_ctx_t adc0;

__attribute__((interrupt()))
void ADC1_2_IRQHandler(void){
    const b8 is_eoc = ADC_GetITStatus(ADC1, ADC_IT_EOC);
    if(is_eoc){
        ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
        BaseType_t bogus = 0;
        xEventGroupSetBitsFromISR(adc0.waiter.cond, 1, &bogus);
    }
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC | ADC_IT_AWD | ADC_IT_JEOC);
    NVIC_ClearPendingIRQ(ADC1_2_IRQn);
}

static
void adc_update_calibval(void){
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    adc0.calibval = Get_CalibrationValue(ADC1);

    ADC_ResetCalibration(ADC1);
}

/**
 * init ADC1 and instance for work
 */
void adc_init(void){
    memclr(&adc0, sizeof(adc_ctx_t));
    minit(adc0.mutex);
    cinit(adc0.waiter);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    /* enable adc irq */
	const NVIC_InitTypeDef adc_nvic = {
		.NVIC_IRQChannel = ADC1_2_IRQn,
		.Priority = 0xf0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&adc_nvic);
    NVIC_ClearPendingIRQ(ADC1_2_IRQn);

    ADC_DeInit(ADC1);
    const ADC_InitTypeDef ADC_InitStructure = {
        .ADC_Mode = ADC_Mode_Independent,
        .ADC_ScanConvMode = DISABLE,
        .ADC_ContinuousConvMode = DISABLE,
        .ADC_ExternalTrigConv = ADC_ExternalTrigConv_None,
        .ADC_DataAlign = ADC_DataAlign_Right,
        .ADC_NbrOfChannel = 1,
    };
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    
    adc_update_calibval();
}

/**
 * get adc value for 1 ADC_Channel
 */
__noinline
u16 adc_get(const u8 ch){
    u16 result = 0;
    mlock(adc0.mutex);

    xEventGroupClearBits(adc0.waiter.cond, 1);

    ADC_RegularChannelConfig(ADC1, ch, 0, ADC_SampleTime_1Cycles5);
    ADC_RegularSequencerLengthConfig(ADC1, 1);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    u32 waited = 0;
    cwait(adc0.waiter, waited);

    result = ADC_GetConversionValue(ADC1);

    ADC_RegularSequencerLengthConfig(ADC1, 0);

    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    ADC_RegularDrop(ADC1);

    xEventGroupClearBits(adc0.waiter.cond, 1);

    mulock(adc0.mutex);
    return (0 != waited)? result : 0;
}

/**
 * get volt value for 1 ADC_Channel
 */
u16 adc_get_volt(const u8 ch){
	const u16 pinval = adc_get(ch);
    
    ADC_TempSensorVrefintCmd(ADC1, ENABLE);
	const u16 vrefval = adc_get(ADC_Channel_Vrefint);
    ADC_TempSensorVrefintCmd(ADC1, DISABLE);

    /**
     * pinval      volt
     * ------   = -------
     * vrefval     1.25v
     * */
    const u32 result = ((u32)pinval * ADC_VREF_VOLT) / (u32)vrefval;

    LOGI("ch: %d|||\t value: %d\t vref: %d\t volt: %d", ch, pinval, vrefval, result);
    return result;
}

/**
 * get temperature by ADC TempSensor
 */
s32 adc_get_temp(void){
    ADC_TempSensorVrefintCmd(ADC1, ENABLE);
    const u16 tempval = adc_get(ADC_Channel_TempSensor);
    ADC_TempSensorVrefintCmd(ADC1, DISABLE);
    const s32 calc_value = (tempval * 3300 / 4096);
    const s32 result = TempSensor_Volt_To_Temper(calc_value);

    return result;
}

