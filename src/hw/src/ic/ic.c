#define IC_MODULE
#include "ic_i.h"

b8 ic_init(THIS, const ic_conf_t * const conf){
	memclr(this, sizeof(ic_t));
    const u8 num = (conf->num - 1);

    u8 awaliable = 0;
    for(u8 i = 0; i < 4; i++){
    	DPORT port = conf->in_port[i];
    	if(NULL != port){
    		awaliable |= (1 << i);
			const u16 mask = (1 << conf->in_pin[i]);
			DPORT_DIN_INIT(port, mask);
			//DPORT_AFIN_INIT(port, mask);
    	}
    }

    this->timer = conf->timer;
    this->num = num;
	this->dma_ch = 0;
    this->awaliable = awaliable;

	/* enable timer irq */
	const NVIC_InitTypeDef timer_nvic = {
		.NVIC_IRQChannel = ic_irqs[num],
		.Priority = 0x10,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&timer_nvic);
	NVIC_ClearPendingIRQ(ic_irqs[num]);

    return true;
}


void ic_prep(THIS, const tim_prep_setups_t setup)
{
	TIM_Cmd(this->timer->hw, DISABLE);
	timer_prep(this->timer, setup);
}

void ic_set_freq(THIS, const u16 div, const u16 psc, const u16 per)
{
	TIM_Cmd(this->timer->hw, DISABLE);
	timer_set_freq(this->timer, div, psc, per);
}


void ic_start_circ_rx(THIS,
                      const u8 ch,
                      const u16 * buff,
                      const u32 data_cnt)
{
    const b8 is_awailable = (0 < (this->awaliable & (1 << ch)));
    if(false == is_awailable){
    	return;
    }
	const u8 num = this->num;
	this->dma_ch = ic_numch2dma_ch[(num * 4) + ch];

	dma_lock(this->dma_ch);
	dma_set_waiting(this->dma_ch, DMA_EBITS_TC_M | DMA_EBITS_HTC_M);

	const TIM_ICInitTypeDef TIM_ICInitStructure = {
		.TIM_Channel = ic_ch2regval[ch],
		.TIM_ICPrescaler = TIM_ICPSC_DIV1,
		.TIM_ICFilter = 0x00,
		.TIM_ICPolarity = TIM_ICPolarity_BothEdge,
		.TIM_ICSelection = TIM_ICSelection_TRC, // 
	};

	TIM_ICInit(this->timer->hw, &TIM_ICInitStructure);

	void * const pa = timer_get_dataptr(num, ch);
	dma_circ_p2mem(this->dma_ch, pa, buff, data_cnt, sizeof(u16));

	TIM_DMACmd(this->timer->hw, ic_dma_source[ch], ENABLE);
	TIM_DMAConfig(this->timer->hw, timer_get_dmabase(ch), TIM_DMABurstLength_1Transfer);

	TIM_SelectInputTrigger(this->timer->hw, TIM_TS_TI1F_ED);
	timer_start(this->timer);
}


void ic_start_rx(THIS,
                 const u8 ch,
                 const u16 * buff,
                 const u32 data_cnt)
{
	const b8 is_awailable = (0 < (this->awaliable & (1 << ch)));
    if(false == is_awailable){
    	return;
    }
	const u8 num = this->num;	
	this->dma_ch = ic_numch2dma_ch[(num * 4) + ch];

	dma_lock(this->dma_ch);
	dma_set_waiting(this->dma_ch, DMA_EBITS_TC_M);

	const TIM_ICInitTypeDef TIM_ICInitStructure = {
		.TIM_Channel = ic_ch2regval[ch],
		.TIM_ICPrescaler = TIM_ICPSC_DIV1,
		.TIM_ICFilter = 0x00,
		.TIM_ICPolarity = TIM_ICPolarity_BothEdge,
		.TIM_ICSelection = TIM_ICSelection_TRC, // 
	};

	TIM_ICInit(this->timer->hw, &TIM_ICInitStructure);

	void * const pa = timer_get_dataptr(num, ch);
	dma_rx_p2mem(this->dma_ch, pa, buff, data_cnt, sizeof(u16));

	TIM_DMACmd(this->timer->hw, ic_dma_source[ch], ENABLE);
	TIM_DMAConfig(this->timer->hw, timer_get_dmabase(ch), TIM_DMABurstLength_1Transfer);

	TIM_SelectInputTrigger(this->timer->hw, TIM_TS_TI1F_ED);
	timer_start(this->timer);
}


u32 ic_stop(THIS){
    timer_stop(this->timer);
    const u32 result = dma_stop(this->dma_ch);

	dma_set_waiting(this->dma_ch, 0);
	dma_ulock(this->dma_ch);
	return result;
}

/**
 * process IC values to real time diffs
 * 
 * @param dtr - [inout] ic values to process
 * @param cnt - [in] size of dtr
 * @param period - [in] period what was used in IC
 * 
 * @return 0 - if not enought data
 * 		   (cnt - 1) - processed size
 */
u32 ic_process(u16 * const data, const u32 cnt, const u16 period) {
	if(cnt < 2){
		/* not enought data */
		return 0;
	}

	u16 last = data[0];

	for(u32 i = 1; i < cnt; i++){
		/* process IC data to time diff */
		const u16 curr = data[i];
		u16 diff = 0;
		if(curr > last){
			diff = curr - last;
		} else {
			diff = period - last + curr;
		}
		data[i - 1] = diff;
		last = curr;
	}

	/* clr last IC data */
	data[cnt - 1] = 0;
	return (cnt - 1);
}
