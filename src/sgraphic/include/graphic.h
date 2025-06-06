#ifndef GRAPHIC_H
#define GRAPHIC_H
#include "ia_arch.h"
#include "sfifo8.h"
#include "graphic_be.h"

#define GRAPHIC_COMBUFF_SIZ (128)
#define GRAPHIC_STACK_SIZ   (256)
#define GRAPHIC_PRIORITY    (8)

typedef enum __packed {
    GC_NONE = 0,
    GC_OFF,
    GC_ON,
    GC_CLEAR,
    GC_EXPOSE,
    GC_SET_CHAR,
    GC_SET_LINE,
    GC_SET_SCREEN,
    GC_LOAD_EXT,
    GC_CNT,
    GC_WITHOUT_ARGS_LAST = GC_EXPOSE,
} gcom_t;

typedef graphic_be_t * (*graphic_be_init_t)(void * const be, const void * const be_conf);

typedef struct {
    graphic_be_init_t be_init;
    void * const be;
    const void * const be_conf;
} graphic_conf_t;

typedef struct {
    graphic_be_t * be;

#ifdef GRAPHIC_THREAD
    const graphic_conf_t * conf;

    sfifo8_t comfifo;
    u8 combuff[GRAPHIC_COMBUFF_SIZ];

    tdm_t mutex;
    tdc_t cond;
    TASK_T(GRAPHIC_STACK_SIZ);
#endif
} graphic_t;



/**
 * init be, graphic and start thread
 */
extern
void graphic_init(graphic_t * const this, const graphic_conf_t * const conf);

/**
 * put com for turn off screen
 */
extern
void graphic_off(graphic_t * const this);

/**
 * put com for turn on screen
 */
extern
void graphic_on(graphic_t * const this);

/**
 * put com for clear screen
 */
extern
void graphic_clear(graphic_t * const this);

/**
 * put com for expose screen data
 */
extern
void graphic_expose(graphic_t * const this);

/**
 * put com for update 1 char on screen
 */
extern
void graphic_set_char(graphic_t * const this, const u8 c, const u8 x, const u8 y);

/**
 * put com for update 1 line on screen
 */
extern
void graphic_set_line(graphic_t * const this, const u8 * line, const u8 len, const u8 y);

/**
 * put com for update multi line on screen (start from 0)
 */
extern
void graphic_set_screen(graphic_t * const this,
						const u8 * const * const lines,
						const u8 * const lens,
						const u8 line_cnt);


/**
 * put com for load extar char to selected pos
 */
extern
void graphic_loadextr(graphic_t * const this,
                      const u8 pos,
                      const u8 ext);

#endif//GRAPHIC_H

#if defined(DEVICE_TREE_H)
extern graphic_t graphics;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(GRAPHIC_DEVICE_TREE)
#define GRAPHIC_DEVICE_TREE

#if !defined(GRAPHIC_BE_INIT) || !defined(GRAPHIC_BE) || !defined(GRAPHIC_BE_CONF)
#error "graphic backend thing not defined"
#endif

graphic_t graphics;
static
const graphic_conf_t graphic0_conf = {
    .be_init = GRAPHIC_BE_INIT,
    .be = GRAPHIC_BE,
    .be_conf = GRAPHIC_BE_CONF,
};
#define GRAPHIC_INIT()		graphic_init(&graphics, &graphic0_conf)
#endif
