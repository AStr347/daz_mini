#ifndef GRAPHIC_MODULE
#error "must be include just into graphic module files"
#endif
#include "graphic.h"
#include "device_tree.h"

#define THIS    graphic_t * const this

#ifdef GRAPHIC_THREAD
#define GTHREAD_INIT()\
{\
	this->conf = conf;\
    const b8 init_res = sfifo8_init(&this->comfifo, this->combuff, GRAPHIC_COMBUFF_SIZ);\
    if(false == init_res){\
        return;\
    }\
    minit(this->mutex);\
    cinit(this->cond);\
    TaskCreate(this, graphic_routine, GRAPHIC_STACK_SIZ, GRAPHIC_PRIORITY);\
}

#define GTHREAD_MLOCK()     mlock(this->mutex)
#define GTHREAD_MULOCK()    mulock(this->mutex)

#define GTHREAD_CMWAIT(__RES__)                 cmwait(this->cond, this->mutex, __RES__)
#define GTHREAD_CMTWAIT(__TIME__, __RES__)      cmtwait(this->cond, this->mutex, __TIME__, __RES__)

#else

#define GTHREAD_INIT()\
{\
    this->be = conf->be_init(conf->be, conf->be_conf);\
}

#define GTHREAD_MLOCK()
#define GTHREAD_MULOCK()

#endif

typedef struct {
    gcom_t com;
    u8 siz;
    u8 data[0];
} graphci_com_base_t;

typedef struct {
    gcom_t com;
    u8 siz;
    u8 c;
    u8 x;
    u8 y;
} graphci_com_char_t;

typedef struct {
    u8 len;
    u8 str[GRAPHIC_BE_COLUMN_CNT];
} graphci_com_line_data_t;

typedef struct {
    gcom_t com;
    u8 siz;
    u8 y;
    u8 data[sizeof(graphci_com_line_data_t)];
} graphci_com_line_t;

typedef struct {
    gcom_t com;
    u8 siz;
    u8 lines_cnt;
    /* row_cnt for lens, row_cnt * column_cnt for lines */
    u8 data[sizeof(graphci_com_line_data_t) * GRAPHIC_BE_ROW_CNT];
} graphci_com_screen_t;

typedef struct {
    gcom_t com;
    u8 siz;
    u8 pos;
    u8 ext;
} graphci_com_loadext_t;

typedef union {
    graphci_com_base_t base;
    graphci_com_char_t cchar;
    graphci_com_line_t cline;
    graphci_com_screen_t cscreen;
    graphci_com_loadext_t cloadext;
} graphci_com_u;

/**
 * execute command by backend
 */
extern
void graphic_com_proc(THIS,
                      const graphci_com_u * const comu);

