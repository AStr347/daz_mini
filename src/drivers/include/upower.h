#ifndef UPOWER_H
#define UPOWER_H
#include "ia_arch.h"
#include "sfifo8.h"
#include "inevent.h"
#include "upower_be.h"

#define UPOWER_STACK_SIZE   (configMINIMAL_STACK_SIZE + 64)
#define UPOWER_PRIORITY     (11)

#define UPOWER_COMBUFF_SIZE (8)


/* sound backend modes */
typedef enum __attribute__((packed)){
	UPOWERM_NONE = 0,
	UPOWERM_MONITORING,
} upower_mode_t;

typedef enum __attribute__((packed)){
    UPOWERC_NOOP = 0,
    UPOWERC_MONITORING,

    UPOWERC_POWERUNLOCK,
    UPOWERC_POWERLOCK,
} upower_com_t;

typedef struct {
    upower_be_init_t be_init;
    void * be;
    const void * be_conf;

    inevent_t * input;
} upower_conf_t;

typedef struct {
    upower_be_t * be;
    const upower_conf_t * conf;
    inevent_t * input;

    tdm_t mutex;
    tdc_t cond;

    sfifo8_t fifo;
    u8 combuff[UPOWER_COMBUFF_SIZE];

    upower_mode_t mode;

    TASK_T(UPOWER_STACK_SIZE);
} upower_t;

extern
upower_levels_t upower_charge(upower_t * const this);

extern
upower_stat_t upower_stat(upower_t * const this);

extern
b8 upower_is_good(upower_t * const this);

extern
void upower_put_com(upower_t * const this, const upower_com_t com);

extern
void upower_init(upower_t * const this, const upower_conf_t * const conf);



#ifdef UPOWER_MODULE

#define this_get_charge()\
(this->be->ft->get_charge(this->be->this))

#define this_get_stat()\
(this->be->ft->get_stat(this->be->this))

#define this_power_is_good()\
(this->be->ft->power_is_good(this->be->this))

#define this_monitor()\
(this->be->ft->monitor(this->be->this))

#define this_plock()\
(this->be->ft->plock(this->be->this))

#define this_punlock()\
(this->be->ft->punlock(this->be->this))

#endif

#endif//UPOWER_H

#if defined(DEVICE_TREE_H)
extern upower_t upower0;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(UPOWER_DEVICE_TREE)
#define UPOWER_DEVICE_TREE

#if !defined(UPOWER_BE_INIT) || !defined(UPOWER_BE) || !defined(UPOWER_BE_CONF)
#error "UPOWER thing not defined"
#endif

upower_t upower0;
static const
upower_conf_t upower_conf = {
    .be_init = UPOWER_BE_INIT,
    .be = UPOWER_BE,
    .be_conf = UPOWER_BE_CONF,

	.input = &INE_DEFAULT,
};
#define UPOWER_INIT()		upower_init(&upower0, &upower_conf)
#endif
