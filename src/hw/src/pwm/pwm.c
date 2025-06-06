#define PWM_MODULE
#include "pwm_i.h"


b8 pwm_init(THIS, const pwm_conf_t * const conf){
	memclr(this, sizeof(pwm_t));
    const u8 num = (conf->num - 1);
    const dma_chnum_t dma_ch = timer_get_dmach(num);

    u8 awaliable = 0;
    for(u8 i = 0; i < 4; i++){
    	DPORT port = conf->out_port[i];
    	if(NULL != port){
    		awaliable |= (1 << i);
			const u16 mask = (1 << conf->out_pin[i]);
			DPORT_AFOUT_INIT(port, mask);
    	}
    }

    this->timer = conf->timer;
    this->num = num;
    this->dma_ch = dma_ch;
    this->awaliable = awaliable;

    return true;
}

/**
 * look to timer_prep
 */
void pwm_prep(THIS, const tim_prep_setups_t setup)
{
	TIM_Cmd(this->timer->hw, DISABLE);
	timer_prep(this->timer, setup);
}

/**
 * look to timer_set_freq
 */
void pwm_set_freq(THIS, const u16 div, const u16 psc, const u16 per)
{
	TIM_Cmd(this->timer->hw, DISABLE);
	timer_set_freq(this->timer, div, psc, per);
}

/**
 * do circular PWM pulses
 *
 * values from buff control part of "lo level"
 *
 * @param this - pwm instance
 * @param ch - num of channel
 * @param buff - data to puls
 * @param data_cnt - length of buff
 *
 */
void pwm_start_circ_tx(THIS,
                       const u8 ch,
                       const u16 * buff,
                       const u32 data_cnt)
{
    const b8 is_awailable = (0 < (this->awaliable & (1 << ch)));
    if(false == is_awailable){
    	return;
    }
	dma_lock(this->dma_ch);

	const u8 num = this->num;
	const TIM_OCInitTypeDef TIM_OCInitStructure = {
		.TIM_OCMode = TIM_OCMode_PWM2,
		.TIM_OutputState = TIM_OutputState_Enable,
		.TIM_Pulse = 0,
		.TIM_OCPolarity = TIM_OCPolarity_High,
	};

	switch (ch){
		case 0:{
			TIM_OC1Init(this->timer->hw, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(this->timer->hw, TIM_OCPreload_Disable);
		}break;

		case 1:{
			TIM_OC2Init(this->timer->hw, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(this->timer->hw, TIM_OCPreload_Disable);
		}break;

		case 2:{
			TIM_OC3Init(this->timer->hw, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(this->timer->hw, TIM_OCPreload_Disable);
		}break;

		case 3:{
			TIM_OC4Init(this->timer->hw, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(this->timer->hw, TIM_OCPreload_Disable);
		}break;
	}

	TIM_ARRPreloadConfig(this->timer->hw, DISABLE);

	void * const pa = timer_get_dataptr(num, ch);
	dma_circ_mem2p(this->dma_ch, pa, buff, data_cnt, sizeof(u16));

	TIM_DMACmd(this->timer->hw, TIM_DMA_Update, ENABLE);
	TIM_DMAConfig(this->timer->hw, timer_get_dmabase(ch), TIM_DMABurstLength_1Transfer);
	TIM_Cmd(this->timer->hw, ENABLE);
	TIM_CtrlPWMOutputs(this->timer->hw, ENABLE);
}

/**
 * do 1 buff transmit
 *
 * values from buff control part of "lo level"
 *
 * @param this - pwm instance
 * @param ch - num of channel
 * @param buff - data to puls
 * @param data_cnt - length of buff
 *
 */
void pwm_start_tx(THIS,
                  const u8 ch,
                  const u16 * buff,
                  const u32 data_cnt)
{
	const b8 is_awailable = (0 < (this->awaliable & (1 << ch)));
	if(false == is_awailable){
		return;
	}
	dma_lock(this->dma_ch);
	const u8 num = this->num;

	const TIM_OCInitTypeDef TIM_OCInitStructure = {
		.TIM_OCMode = TIM_OCMode_PWM2,
		.TIM_OutputState = TIM_OutputState_Enable,
		.TIM_Pulse = 0,
		.TIM_OCPolarity = TIM_OCPolarity_High
	};

	switch (ch){
		case 0:{
			TIM_OC1Init(this->timer->hw, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(this->timer->hw, TIM_OCPreload_Disable);
		}break;

		case 1:{
			TIM_OC2Init(this->timer->hw, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(this->timer->hw, TIM_OCPreload_Disable);
		}break;

		case 2:{
			TIM_OC3Init(this->timer->hw, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(this->timer->hw, TIM_OCPreload_Disable);
		}break;

		case 3:{
			TIM_OC4Init(this->timer->hw, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(this->timer->hw, TIM_OCPreload_Disable);
		}break;
	}

	TIM_ARRPreloadConfig(this->timer->hw, DISABLE);

	void * const pa = timer_get_dataptr(num, ch);
	dma_tx_mem2p(this->dma_ch, pa, buff, data_cnt, sizeof(u16));

	TIM_DMACmd(this->timer->hw, TIM_DMA_Update, ENABLE);
	TIM_DMAConfig(this->timer->hw, timer_get_dmabase(ch), TIM_DMABurstLength_1Transfer);
	TIM_Cmd(this->timer->hw, ENABLE);
	TIM_CtrlPWMOutputs(this->timer->hw, ENABLE);
}


void pwm_stop(THIS){
	TIM_CtrlPWMOutputs(this->timer->hw, DISABLE);
	TIM_Cmd(this->timer->hw, DISABLE);
    dma_stop(this->dma_ch);

	dma_ulock(this->dma_ch);
}

