#define TIMER_MODULE
#include "timer_i.h"
#include "log.h"

void timer_init(THIS, const timer_conf_t * const conf){
    memclr(this, sizeof(ia_timer_t));
    const u8 num = (conf->num - 1);
    if(0 == num){
        RCC_APB2PeriphClockCmd(num2apb[num], ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(num2apb[num], ENABLE);
    }

    minit(this->mutex);
    cinit(this->cond);

    this->hw = timers[num];
    this->num = num;
    this->freq = 1;
    this->timclk = SystemCoreClock;//(SystemCoreClock / 2);

    TIM_CtrlPWMOutputs(this->hw, DISABLE);
	TIM_Cmd(this->hw, DISABLE);

	/* enable timer irq */
	const NVIC_InitTypeDef timer_nvic = {
		.NVIC_IRQChannel = timer_irqs[num],
		.Priority = 0x10,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&timer_nvic);
	NVIC_ClearPendingIRQ(timer_irqs[num]);
	TIM_ClearITPendingBit(this->hw, 0xff);
}

/**
 * setup timer settings by enum
 * 	_____
 * |     |     |
 * |     |_____|
 * <----->
 *  width
 *
 * values from buff control part of "lo level"
 *
 * @param this - pwm instance
 * @param setup -
 * 					TPSET_250NS - width=250ns
 * 					TPSET_1US - width=1us
 * 					TPSET_10US - width=10us
 * 					TPSET_50US - width=50us
 * 					e.t.c
 *
 */
void timer_prep(THIS, const tim_prep_setups_t setup)
{
	timer_set_freq(this, divs[setup], pscs[setup], pers[setup]);
}

/**
 * width of 1 PWM pulse in Hz:
 * width = (SYSCLK / div / (prescaler + 1) / (period + 1))
 * ??? real freq will be x2 ???
 * better use timer_prep
 *
 * 	_____
 * |     |     |
 * |     |_____|
 * <----------->
 *     width
 */
void timer_set_freq(THIS, const u16 div, const u16 psc, const u16 per)
{
    const TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {
		.TIM_Period = per,
		.TIM_Prescaler = psc,
		.TIM_ClockDivision = div,
		.TIM_CounterMode = TIM_CounterMode_Up,
		.TIM_RepetitionCounter = 0,
	};
	TIM_TimeBaseInit(this->hw, &TIM_TimeBaseInitStructure);
}

b8 timer_twait(THIS, const u16 cnt){
	return true;
//	const u32 wait_val = this->freq / cnt;
//	timer_set_freq(this, wait_val);
//
//	xEventGroupClearBits(this->cond.cond, TIM_IT_Update);
//	TIM_ClearITPendingBit(this->hw, TIM_IT_Update);
//	NVIC_ClearPendingIRQ(timer_irqs[this->num]);
//
//	TIM_SetCounter(this->hw, 0);
//	/* for timer update 1 times, ATRLR = 0 */
//	TIM_SetAutoreload(this->hw, 0);
//
//	TIM_ITConfig(this->hw, TIM_IT_Update, ENABLE);
//	TIM_Cmd(this->hw, ENABLE);
//
//	const u32 waited = xEventGroupWaitBits(this->cond.cond, TIM_IT_Update, true, false, portMAX_DELAY);
//
//	TIM_Cmd(this->hw, DISABLE);
//	TIM_ITConfig(this->hw, TIM_IT_Update, DISABLE);
//
//	return 0 != waited;
}

void timer_off(THIS){
	TIM_CtrlPWMOutputs(this->hw, DISABLE);
	TIM_Cmd(this->hw, DISABLE);
	TIM_ITConfig(this->hw, 0xff, DISABLE);
	this->freq = 1;
}

void * timer_get_dataptr(const u8 num, const u8 ch)
{
    return timer_numch2dataptr[(num * 4) + ch];
}

u16 timer_get_dmabase(const u8 ch)
{
    return timer_ch2dmabase[ch];
}

dma_chnum_t timer_get_dmach(const u8 num){
    return timer_numch2dma_ch[num];
}

static inline
void timer_irq_handler(THIS){
	__unused
	const u16 it = this->hw->INTFR;
	TIM_ClearITPendingBit(this->hw, 0xff);
	// const b8 is_update = (0 < (it & TIM_IT_Update));
	// const b8 is_cc1 = (0 < (it & TIM_IT_CC1));
	// const b8 is_cc2 = (0 < (it & TIM_IT_CC2));
	// const b8 is_cc3 = (0 < (it & TIM_IT_CC3));
	// const b8 is_cc4 = (0 < (it & TIM_IT_CC4));
	// const b8 is_com = (0 < (it & TIM_IT_COM));
	// const b8 is_trigger = (0 < (it & TIM_IT_Trigger));
	// const b8 is_break = (0 < (it & TIM_IT_Break));
}

#ifdef TIMER1_HW
extern ia_timer_t timer1;
__attribute__((interrupt()))
void TIM1_IRQHandler(void) { timer_irq_handler(&timer1); }
#endif
#ifdef TIMER2_HW
extern ia_timer_t timer2;
__attribute__((interrupt()))
void TIM2_IRQHandler(void) { timer_irq_handler(&timer2); }
#endif
#ifdef TIMER3_HW
extern ia_timer_t timer3;
__attribute__((interrupt()))
void TIM3_IRQHandler(void) { timer_irq_handler(&timer3); }
#endif
#ifdef TIMER4_HW
extern ia_timer_t timer4;
__attribute__((interrupt()))
void TIM4_IRQHandler(void) { timer_irq_handler(&timer4); }
#endif




