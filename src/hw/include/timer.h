#ifndef TIMER_H
#define TIMER_H

#include "ia_arch.h"
#include "dma.h"

/* peripherial alias */
typedef TIM_TypeDef* TIMER;

typedef enum {
	TPSET_OC_MAXIMUM_1PER = 0,
	TPSET_OC_250NS_1PER,
	TPSET_OC_1US_1PER,
	TPSET_OC_10US_1PER,
	TPSET_OC_50US_1PER,

	TPSET_IC_MAXIMUM,
	TPSET_IC_1US,

	TPSET_CNT,
} tim_prep_setups_t;

typedef struct {
    u8 num;
} timer_conf_t;

typedef struct {
    TIMER hw;
    tdm_t mutex;
    tdc_t cond;
    u32 timclk;

    u8 num;
    u32 freq;
} ia_timer_t;

extern
void timer_init(ia_timer_t * const this, const timer_conf_t * const conf);

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
extern
void timer_prep(ia_timer_t * const this, const tim_prep_setups_t setup);

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
extern
void timer_set_freq(ia_timer_t * const this, const u16 div, const u16 psc, const u16 per);

extern
b8 timer_twait(ia_timer_t * const this, const u16 cnt);

extern
void timer_off(ia_timer_t * const this);

extern
void * timer_get_dataptr(const u8 num, const u8 ch);

extern
u16 timer_get_dmabase(const u8 ch);

extern
dma_chnum_t timer_get_dmach(const u8 num);

static inline
void timer_start(ia_timer_t * const this){
	TIM_SetCounter(this->hw, 0);
	TIM_Cmd(this->hw, ENABLE);
}

static inline
void timer_stop(ia_timer_t * const this){
	TIM_Cmd(this->hw, DISABLE);
}

static inline
void timer_lock(ia_timer_t * const this){
    mlock(this->mutex);
}

static inline
void timer_unlock(ia_timer_t * const this){
    mulock(this->mutex);
}

static inline
void timer_wait(ia_timer_t * const this){
    __unused
    u32 waited = 0;
    cwait(this->cond, waited);
}

#endif//TIMER_H

#if defined(DEVICE_TREE_H)
#ifdef TIMER1_HW
extern ia_timer_t timer1;
#endif
#ifdef TIMER2_HW
extern ia_timer_t timer2;
#endif
#ifdef TIMER3_HW
extern ia_timer_t timer3;
#endif
#ifdef TIMER4_HW
extern ia_timer_t timer4;
#endif
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(TIMER_DEVICE_TREE)
#define TIMER_DEVICE_TREE

#ifdef TIMER1_HW
ia_timer_t timer1;
static const
timer_conf_t timer1_conf = {
    .num = 1,
};
#define TIMER1_INIT()		    timer_init(&timer1, &timer1_conf);
#else
#define TIMER1_INIT()
#endif

#ifdef TIMER2_HW
ia_timer_t timer2;
static const
timer_conf_t timer2_conf = {
    .num = 2,
};
#define TIMER2_INIT()		    timer_init(&timer2, &timer2_conf);
#else
#define TIMER2_INIT()
#endif

#ifdef TIMER3_HW
ia_timer_t timer3;
static const
timer_conf_t timer3_conf = {
    .num = 3,
};
#define TIMER3_INIT()		    timer_init(&timer3, &timer3_conf);
#else
#define TIMER3_INIT()
#endif

#ifdef TIMER4_HW
ia_timer_t timer4;
static const
timer_conf_t timer4_conf = {
    .num = 4,
};
#define TIMER4_INIT()		    timer_init(&timer4, &timer4_conf);
#else
#define TIMER4_INIT()
#endif



#endif
