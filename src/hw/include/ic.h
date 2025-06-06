#ifndef IA_IC_H
#define IA_IC_H
#include "timer.h"
#include "dma.h"
#include "dport.h"

typedef struct {
    ia_timer_t * timer;
    DPORT in_port[4];   
    u8 in_pin[4];
    u8 num;
} ic_conf_t;

typedef struct {
    ia_timer_t * timer;
    u8 num;
    u8 awaliable;
    dma_chnum_t dma_ch;
} ic_t;

extern
b8 ic_init(ic_t * const this, const ic_conf_t * const conf);

extern
void ic_prep(ic_t * const this, const tim_prep_setups_t setup);

extern
void ic_set_freq(ic_t * const this, const u16 div, const u16 psc, const u16 per);

extern
void ic_start_circ_rx(ic_t * const this,
                       const u8 ch,
                       const u16 * buff,
                       const u32 data_cnt);

extern
void ic_start_rx(ic_t * const this,
                  const u8 ch,
                  const u16 * buff,
                  const u32 data_cnt);

extern
u32 ic_stop(ic_t * const this);

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
extern
u32 ic_process(u16 * const data, const u32 cnt, const u16 period);

static inline
void ic_lock(ic_t * const this){
	timer_lock(this->timer);
}

static inline
void ic_unlock(ic_t * const this){
	timer_unlock(this->timer);
}

static inline
b8 ic_wait(ic_t * const this, const u32 msec){
	const dma_ebits_t res = dma_twait(this->dma_ch, DMA_EBITS_TC_M, ms2tick(msec));
	return (0 == res)? false : true;
}

#endif//IC_H

#if defined(DEVICE_TREE_H)
#ifdef IC1_HW
extern ic_t ic1;
#endif
#ifdef IC2_HW
extern ic_t ic2;
#endif
#ifdef IC3_HW
extern ic_t ic3;
#endif
#ifdef IC4_HW
extern ic_t ic4;
#endif
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(IC_DEVICE_TREE)
#define IC_DEVICE_TREE

#ifdef IC1_HW
ic_t ic1;
static const
ic_conf_t ic1_conf = {
    .in_port = {
    		[0] = IC1_HW_PORT0,
			[1] = IC1_HW_PORT1,
			[2] = IC1_HW_PORT2,
			[3] = IC1_HW_PORT3,
    },
    .in_pin = {
    		[0] = IC1_HW_PIN0,
			[1] = IC1_HW_PIN1,
			[2] = IC1_HW_PIN2,
			[3] = IC1_HW_PIN3,
    },
    .num = 1,
	.timer = &timer1,
};
#define IC1_INIT()		    ic_init(&ic1, &ic1_conf);
#else
#define IC1_INIT()
#endif

#ifdef IC2_HW
ic_t ic2;
static const
ic_conf_t ic2_conf = {
    .in_port = {
    		[0] = IC2_HW_PORT0,
			[1] = IC2_HW_PORT1,
			[2] = IC2_HW_PORT2,
			[3] = IC2_HW_PORT3,
    },
    .in_pin = {
    		[0] = IC2_HW_PIN0,
			[1] = IC2_HW_PIN1,
			[2] = IC2_HW_PIN2,
			[3] = IC2_HW_PIN3,
    },
    .num = 2,
	.timer = &timer2,
};
#define IC2_INIT()		    ic_init(&ic2, &ic2_conf);
#else
#define IC2_INIT()
#endif

#ifdef IC3_HW
ic_t ic3;
static const
ic_conf_t ic3_conf = {
    .in_port = {
    		[0] = IC3_HW_PORT0,
			[1] = IC3_HW_PORT1,
			[2] = IC3_HW_PORT2,
			[3] = IC3_HW_PORT3,
    },
    .in_pin = {
    		[0] = IC3_HW_PIN0,
			[1] = IC3_HW_PIN1,
			[2] = IC3_HW_PIN2,
			[3] = IC3_HW_PIN3,
    },
    .num = 3,
	.timer = &timer3,
};
#define IC3_INIT()		    ic_init(&ic3, &ic3_conf);
#else
#define IC3_INIT()
#endif

#ifdef IC4_HW
ic_t ic4;
static const
ic_conf_t ic4_conf = {
    .in_port = {
    		[0] = IC4_HW_PORT0,
			[1] = IC4_HW_PORT1,
			[2] = IC4_HW_PORT2,
			[3] = IC4_HW_PORT3,
    },
    .in_pin = {
    		[0] = IC4_HW_PIN0,
			[1] = IC4_HW_PIN1,
			[2] = IC4_HW_PIN2,
			[3] = IC4_HW_PIN3,
    },
    .num = 4,
	.timer = &timer4,
};
#define IC4_INIT()		    ic_init(&ic4, &ic4_conf);
#else
#define IC4_INIT()
#endif
#endif
