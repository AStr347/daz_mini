/**
 * contain interface to work with radio
 */
#ifndef LINK_LEGACY_IFACE_H
#define LINK_LEGACY_IFACE_H
#include "ia_arch.h"
#include "slice.h"

/* forward declaration */
typedef b8 (*ll_be_transmit_t)(void * const this,
							   const u8 cnt,
							   const slice_t* slices,
							   const u8 repeat_cnt);

typedef s32 (*ll_be_receive_t)(void * const this,
							   slice_t * restrict slices,
							   const u32 size,
							   const u32 msec);

/**
 * turn device to sleep mode
 * @param be		- backend
 * @return		- true if success
 */
typedef b8(*ll_be_sleep_t) (void * const this);
/**
 * wake device from sleep
 * @param be		- backend
 * @return		- true if success
 */
typedef b8(*ll_be_wake_t) (void * const this);

/**
 * turn device off
 * @param be		- backend
 * @return		- true if success
 */
typedef b8(*ll_be_off_t) (void * const this);

/**
 * turn device on
 * @param be		- backend
 * @return		- true if success
 */
typedef b8(*ll_be_on_t) (void * const this);

/**
 * set central modulation freq
 * @param be		- backend
 * @param freq		- freq identifier
 * @return 		- true if success
 */
//typedef b8(*ll_be_freq_t) (void * const this, const u32 freq);

/**
 * legacy funtion table
 * @mem transmit	- transmit data through legacy interface 
 * @mem recceive	- recceive data through legacy interface
 * @mem sleep		- turn off tranceiver
 * @mem wake		- turn on tranceiver
 * @mem freq 		- set central freq
 * @mem on		- turn device on
 * @mem off		- turn device off
 */
typedef struct {
	ll_be_transmit_t transmit;
	ll_be_receive_t receive;
	ll_be_sleep_t sleep;
	ll_be_wake_t wake;
	//ll_be_freq_t freq;
	ll_be_on_t on;
	ll_be_off_t off;
} ll_be_ft_t;

typedef struct {
	const ll_be_ft_t * lft;
	void * this;
} ll_be_t;

/**
 * init_backend
 * @param be		- backend
 * @param freq		- freq identifier
 * @return 		- true if success
 */
typedef ll_be_t* (*ll_be_init_t) (void * const this, const void * const conf);

#ifdef LINK_LEGACY_MODULE
#define this_be_transmit(__CNT__, __SLICES__, __REPEAT__) 	(this->be->lft->transmit(this->be->this, __CNT__, __SLICES__, __REPEAT__))
#define this_be_receive(__SLICES__, __SIZE__, __MSEC__) 	(this->be->lft->receive(this->be->this, __SLICES__, __SIZE__, __MSEC__))
#define this_be_sleep() 									(this->be->lft->sleep(this->be->this))
#define this_be_wake() 										(this->be->lft->wake(this->be->this))
//#define this_be_freq(__FREQ__) 								(this->be->lft->freq(this->be->this, __FREQ__))
#define this_be_on() 										(this->be->lft->on(this->be->this))
#define this_be_off() 										(this->be->lft->off(this->be->this))
#endif

#endif
