#ifndef PWM_H
#define PWM_H
#include "ia_arch.h"
#include "dport.h"
#include "dma.h"
#include "timer.h"

typedef struct {
    DPORT out_port[4];
    u8 out_pin[4];
    u8 num;

    ia_timer_t * timer;
} pwm_conf_t;

typedef struct {
    ia_timer_t * timer;

    dma_chnum_t dma_ch;
    u8 num;
    u8 awaliable;
} pwm_t;

extern
b8 pwm_init(pwm_t * const this, const pwm_conf_t * const conf);

extern
void pwm_prep(pwm_t * const this, const tim_prep_setups_t setup);

extern
void pwm_set_freq(pwm_t * const this, const u16 div, const u16 psc, const u16 per);

extern
void pwm_start_circ_tx(pwm_t * const this,
               	       const u8 ch,
					   const u16 * buff,
					   const u32 data_cnt);

extern
void pwm_start_tx(pwm_t * const this,
				  const u8 ch,
				  const u16 * buff,
				  const u32 data_cnt);

extern
void pwm_stop(pwm_t * const this);

static inline
void pwm_lock(pwm_t * const this){
	timer_lock(this->timer);
}

static inline
void pwm_unlock(pwm_t * const this){
	timer_unlock(this->timer);
}

static inline
b8 pwm_wait(pwm_t * const this) {
	__unused
    const dma_ebits_t res = dma_wait(this->dma_ch, DMA_EBITS_ALL_M);
    return true;
}

#endif//PWM_H

#if defined(DEVICE_TREE_H)
#ifdef PWM1_HW
extern pwm_t pwm1;
#endif
#ifdef PWM2_HW
extern pwm_t pwm2;
#endif
#ifdef PWM3_HW
extern pwm_t pwm3;
#endif
#ifdef PWM4_HW
extern pwm_t pwm4;
#endif
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(PWM_DEVICE_TREE)
#define PWM_DEVICE_TREE

#ifdef PWM1_HW
pwm_t pwm1;
static const
pwm_conf_t pwm1_conf = {
    .out_port = {
    		[0] = PWM1_HW_PORT0,
			[1] = PWM1_HW_PORT1,
			[2] = PWM1_HW_PORT2,
			[3] = PWM1_HW_PORT3,
    },
    .out_pin = {
    		[0] = PWM1_HW_PIN0,
			[1] = PWM1_HW_PIN1,
			[2] = PWM1_HW_PIN2,
			[3] = PWM1_HW_PIN3,
    },
    .num = 1,
	.timer = &timer1,
};
#define PWM1_INIT()		    pwm_init(&pwm1, &pwm1_conf);
#else
#define PWM1_INIT()
#endif

#ifdef PWM2_HW
pwm_t pwm2;
static const
pwm_conf_t pwm2_conf = {
	.out_port = {
			[0] = PWM2_HW_PORT0,
			[1] = PWM2_HW_PORT1,
			[2] = PWM2_HW_PORT2,
			[3] = PWM2_HW_PORT3,
	},
	.out_pin = {
			[0] = PWM2_HW_PIN0,
			[1] = PWM2_HW_PIN1,
			[2] = PWM2_HW_PIN2,
			[3] = PWM2_HW_PIN3,
	},
	.num = 2,
	.timer = &timer2,
};
#define PWM2_INIT()		    pwm_init(&pwm2, &pwm2_conf);
#else
#define PWM2_INIT()
#endif

#ifdef PWM3_HW
pwm_t pwm3;
static const
pwm_conf_t pwm3_conf = {
	.out_port = {
			[0] = PWM3_HW_PORT0,
			[1] = PWM3_HW_PORT1,
			[2] = PWM3_HW_PORT2,
			[3] = PWM3_HW_PORT3,
	},
	.out_pin = {
			[0] = PWM3_HW_PIN0,
			[1] = PWM3_HW_PIN1,
			[2] = PWM3_HW_PIN2,
			[3] = PWM3_HW_PIN3,
	},
	.num = 3,
	.timer = &timer3,
};
#define PWM3_INIT()		    pwm_init(&pwm3, &pwm3_conf);
#else
#define PWM3_INIT()
#endif

#ifdef PWM4_HW
pwm_t pwm4;
static const
pwm_conf_t pwm4_conf = {
	.out_port = {
			[0] = PWM4_HW_PORT0,
			[1] = PWM4_HW_PORT1,
			[2] = PWM4_HW_PORT2,
			[3] = PWM4_HW_PORT3,
	},
	.out_pin = {
			[0] = PWM4_HW_PIN0,
			[1] = PWM4_HW_PIN1,
			[2] = PWM4_HW_PIN2,
			[3] = PWM4_HW_PIN3,
	},
	.num = 4,
	.timer = &timer4,
};
#define PWM4_INIT()		    pwm_init(&pwm4, &pwm4_conf);
#else
#define PWM4_INIT()
#endif

#endif
