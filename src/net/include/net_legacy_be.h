/**
 * contain interface for link layer
 */
#ifndef NET_LEGACY_IFACE_H
#define NET_LEGACY_IFACE_H
#include "ia_arch.h"
#include "netlayer_generic_iface.h"

/**
 * transmit legacy data
 * @param ll		- [inout] linklayer instance
 * @param id		- [in] destination identifier
 * @param data		- [in] data to transmit
 * @return 		- true if success
 * 			  false else
 */
typedef b8(*ll_transmit_t)(void * const this, const u64 id, const net_data_t* restrict const data);

/**
 * @brief recceive data
 * @param ll            - [inout] linklayer instance
 * @param data          - [out] recceived data buffer,
 * @param data_size     - [in] out data size (in entries, value over 4 is useless)
 * @return              - count of data received
 */
typedef	u32 (*ll_receive_t)(void * const this,
                             net_rx_t* restrict const data,
                             const u8 data_size);

/* turn on legacy network wrapper for radio backend
 * @param ll		- [inout] linklayer instance
 */
typedef	void(*ll_on_t)(void * const this);

/* turn off legacy network wrapper for radio backend
 * @param ll		- [inout] linklayer instance
 */
typedef	void(*ll_off_t)(void * const this);

/**
 * legacy funtion table
 * @mem transmit	- transmit data through legacy interface 
 * @mem recceive	- recceive data through legacy interface
 * @mem on  		- turn device on
 * @mem off		- turn device off
 */
typedef struct {
	ll_transmit_t transmit;
	ll_receive_t receive;
	ll_on_t on;
	ll_off_t off;
} net_legacy_be_ft_t;

typedef struct {
	const net_legacy_be_ft_t * ft;
	void * this;
} net_legacy_be_t;

#ifdef NET_MODULE
#define this_ll_transmit(__ID__, __DATA__) 		(this->ll->ft->transmit(this->ll->this, __ID__, __DATA__))
#define this_ll_receive(__DATA__, __SIZE__) 	(this->ll->ft->receive(this->ll->this, __DATA__, __SIZE__))
#define this_ll_on() 							(this->ll->ft->on(this->ll->this))
#define this_ll_off() 							(this->ll->ft->off(this->ll->this))
#endif

/**
 * init legacy linklayer
 * @param ll		- legacy linlayer instance
 * @param conf		- config
 */
typedef net_legacy_be_t* (*ll_init_t)(void * const this, const void * const conf);

#endif
