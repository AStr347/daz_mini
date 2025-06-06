#ifndef CMT2300A_H
#define CMT2300A_H
#include "ia_arch.h"
#include "dport.h"
#include "spi.h"
#include "link_legacy_be.h"
#include "ic.h"
#include "pwm.h"

typedef struct {
    spi_t * spi;
    DPORT int_port;
    DPORT cs_port;
    DPORT pow_port;
    ic_t * ic;
	pwm_t * pwm;

    u8 int_pin;
    u8 cs_pin;
    u8 pow_pin;
} cmt2300a_conf_t;

typedef struct {
    ll_be_t ll_be;
    
    spi_t * spi;
    DPORT int_port;
    DPORT cs_port;
    DPORT pow_port;
    ic_t * ic;
	pwm_t * pwm;

    u16 int_mask;
    u16 cs_mask;
    u16 pow_mask;
} cmt2300a_t;

ll_be_t* cmt2300a_init(cmt2300a_t * const this, const cmt2300a_conf_t * const conf);

#endif//CMT2300A_H

#if defined(DEVICE_TREE_H)
extern cmt2300a_t cmt2300a0;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(CMT2300A_DEVICE_TREE)
#define CMT2300A_DEVICE_TREE

cmt2300a_t cmt2300a0;
static const
cmt2300a_conf_t cmt2300a_conf0 = {
	.spi = CMT2300A_SPI_HW,
    .int_port = CMT2300A_INT_PORT,
    .cs_port = CMT2300A_CS_PORT,
    .pow_port = CMT2300A_POW_PORT,
	.ic = CMT2300A_IC,
	.pwm = CMT2300A_PWM,

    .int_pin = CMT2300A_INT_PIN,
    .cs_pin = CMT2300A_CS_PIN,
    .pow_pin = CMT2300A_POW_PIN,
};

#define LINK_LEGACY_BE_INIT     cmt2300a_init
#define LINK_LEGACY_BE          (&cmt2300a0)
#define LINK_LEGACY_BE_CONF     (&cmt2300a_conf0)
#endif
