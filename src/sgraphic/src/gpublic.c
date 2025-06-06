#define GRAPHIC_MODULE
#include "graphic_i.h"

#ifdef GRAPHIC_THREAD
static
b8 graphic_com_put(THIS, const u8 * data, const u8 siz){
    mlock(this->mutex);

    const b8 result = sfifo8_put(&this->comfifo, data, siz);
    if(result){
        csignal(this->cond);
    }

    mulock(this->mutex);
    return result;
}
#else
static
b8 graphic_com_put(THIS, const graphci_com_u * const comu, const u8 siz){
    GTHREAD_MLOCK();
    graphic_com_proc(this, comu);
    GTHREAD_MULOCK()
}
#endif

/**
 * put com for turn off screen
 */
void graphic_off(THIS){
    static const graphci_com_base_t com = {
        .com = GC_OFF,
        .siz = 0,
    };
    (void)graphic_com_put(this, &com, 1);
}

/**
 * put com for turn on screen
 */
void graphic_on(THIS){
    static const graphci_com_base_t com = {
        .com = GC_ON,
        .siz = 0,
    };
    (void)graphic_com_put(this, &com, 1);
}

/**
 * put com for clear screen
 */
void graphic_clear(THIS){
    static const graphci_com_base_t com = {
        .com = GC_CLEAR,
        .siz = 0,
    };
    (void)graphic_com_put(this, &com, 1);
}

/**
 * put com for clear screen
 */
void graphic_expose(THIS){
    static const graphci_com_base_t com = {
        .com = GC_EXPOSE,
        .siz = 0,
    };
    (void)graphic_com_put(this, &com, 1);
}

/**
 * put com for update 1 char on screen
 */
void graphic_set_char(THIS, const u8 c, const u8 x, const u8 y){
    if(GRAPHIC_BE_COLUMN_CNT < x || GRAPHIC_BE_ROW_CNT < y){
        return;
    }

    const graphci_com_char_t com = {
        .com = GC_SET_CHAR,
        .siz = sizeof(graphci_com_char_t) - sizeof(graphci_com_base_t),
        .c = c,
        .x = x,
        .y = y,
    };
    graphic_com_put(this, &com, sizeof(com));
}

static
u8* fill_line_data(u8* dst, const u8 * line, const u8 len){
    memclr(dst, sizeof(graphci_com_line_data_t));
    graphci_com_line_data_t * const dst_obj = (graphci_com_line_data_t*)dst;
    dst_obj->len = len;
    memcpy(dst_obj->str, line, len);
    return (((u8*) dst) + len + 1);
}

/**
 * put com for update 1 line on screen
 */
void graphic_set_line(THIS, const u8 * line, const u8 len, const u8 y){
    if(GRAPHIC_BE_COLUMN_CNT < len || GRAPHIC_BE_ROW_CNT < y){
        return;
    }

    graphci_com_line_t com = {
        .com = GC_SET_LINE,
        .y = y,
    };

    const u8 * const end = fill_line_data(com.data, line, len);
    const u8 * const ptr = (u8 *)&com;
    const u16 rsiz = (((u32)end - (u32)ptr));

    com.siz = rsiz - 2;
    (void)graphic_com_put(this, &com, rsiz);
}

/**
 * put com for update multi line on screen (start from 0)
 */
void graphic_set_screen(THIS,
						const u8 * const * const lines,
						const u8 * const lens,
						const u8 line_cnt)
{
    if(GRAPHIC_BE_ROW_CNT < line_cnt || 0 == line_cnt){
        return;
    }
    
    graphci_com_screen_t com = {
        .com = GC_SET_SCREEN,
        .lines_cnt = line_cnt,
    };

    /* it lines_cnt */
    u8 * dst = com.data;
    for(u8 i = 0; i < line_cnt; i++){
        dst = fill_line_data(dst, lines[i], lens[i]);
    }

    const u8 * end = dst;
	const u8 * const ptr = (u8 *)&com;
	const u16 rsiz = (((u32)end - (u32)ptr));

	com.siz = rsiz - 2;
	(void)graphic_com_put(this, ptr, rsiz);
}

void graphic_loadextr(graphic_t * const this,
                      const u8 pos,
                      const u8 ext)
{
    if(8 <= pos){
        return;
    }
    const graphci_com_loadext_t com = {
        .com = GC_LOAD_EXT,
        .siz = 2,
        .pos = pos,
        .ext = ext,
    };

    (void)graphic_com_put(this, &com, sizeof(com));
}
