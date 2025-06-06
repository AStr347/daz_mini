#ifndef GRAPHIC_BE_H
#define GRAPHIC_BE_H
#include "ia_arch.h"

typedef void (*graphic_be_off_t)(void * const be);

typedef void (*graphic_be_on_t)(void * const be);

typedef void (*graphic_be_clear_t)(void * const be);

typedef void (*graphic_be_set_char_t)(void * const be,
                                    const u8 c,
                                    const u8 x,
                                    const u8 y);

typedef void (*graphic_be_set_line_t)(void * const be,
                                    const u8 * line,
                                    const u8 len,
                                    const u8 y);

typedef void (*graphic_be_set_screen_t)(void * const be,
                                      const u8 ** lines,
                                      const u8 * lens,
                                      const u8 lines_cnt);

typedef void (*graphic_be_expose_t)(void * const be);

typedef void (*graphic_be_loadextr_t)(void * const be,
                                     const u8 pos,
                                     const u8 ext);


typedef struct {
    graphic_be_off_t off;
    graphic_be_on_t on;
    graphic_be_clear_t clear;
    graphic_be_set_char_t set_char;
    graphic_be_set_line_t set_line;
    graphic_be_set_screen_t set_screen;
    graphic_be_expose_t expose;

    graphic_be_loadextr_t loadextr;
} graphic_be_ft_t;

typedef struct {
    void * this;
    const graphic_be_ft_t * ft;
    u8 rowcnt;
    u8 columncnt;
} graphic_be_t;

#ifdef GRAPHIC_MODULE

#define this_be_off()\
(this->be->ft->off(this->be->this))

#define this_be_on()\
(this->be->ft->on(this->be->this))

#define this_be_clear()\
(this->be->ft->clear(this->be->this))

#define this_be_set_char(__C__, __X__, __Y__)\
(this->be->ft->set_char(this->be->this, __C__, __X__, __Y__))

#define this_be_set_line(__LINE__, __LEN__, __Y__)\
(this->be->ft->set_line(this->be->this, __LINE__, __LEN__, __Y__))

#define this_be_set_screen(__LINES__, __LENS__, __LINES_CNT__)\
(this->be->ft->set_screen(this->be->this, __LINES__, __LENS__, __LINES_CNT__))

#define this_be_expose()\
(this->be->ft->expose(this->be->this))

#define this_be_loadextr(__POS__, __EXT__)\
(this->be->ft->loadextr(this->be->this, __POS__, __EXT__))

#endif

#endif//GRAPHIC_BE_H
