#ifndef BCB0802_MODULE
#error "must be include just into bcb0802.c"
#endif

#include "bcb0802.h"

typedef enum {
    ST7066_CLEAR = (1 << 0),
    ST7066_RETURN = (1 << 1),
    
    ST7066_ENTRY = (1 << 2),
    ST7066_ENTRY_INC = (1 << 1),
    ST7066_ENTRY_DEC = (0 << 1),

    ST7066_DISP = (1 << 3),
    ST7066_ON = (1 << 2),
    ST7066_OFF = (0 << 2),

    ST7066_INIT = (1 << 5),
    ST7066_INIT_8BIT = (1 << 4),
    ST7066_INIT_4BIT = (0 << 4),
    ST7066_INIT_2LINE = (1 << 3),
    ST7066_INIT_1LINE = (0 << 3),
    ST7066_INIT_5X11FONT = (1 << 2),
    ST7066_INIT_5X8FONT = (0 << 2),
    
    ST7066_CGRAM = (1 << 6),
    ST7066_DDRAM = (1 << 7),

    ST7066_ROW_P = 6,
} st7066_coms_t;


#define THIS    bcb0802_t * const this

static
void bcb0802_off(THIS);

static
void bcb0802_on(THIS);

static
void bcb0802_clear(THIS);

static
void bcb0802_set_char(THIS,
                      const u8 c,
                      const u8 x,
                      const u8 y);

static
void bcb0802_set_line(THIS,
                      const u8 * line,
                      const u8 len,
                      const u8 y);

static
void bcb0802_set_screen(THIS,
                        const u8 ** lines,
                        const u8 * lens,
                        const u8 lines_cnt);

static
void bcb0802_expose(THIS);

/**
 * load extar char to selected pos
 */
static
void bcb0802_loadextr(THIS, const u8 pos, const bcb0802_extsymb_t ext);


static const
graphic_be_ft_t bcb0802_ft = {
    .off = (graphic_be_off_t)bcb0802_off,
    .on = (graphic_be_on_t)bcb0802_on,
    .clear = (graphic_be_clear_t)bcb0802_clear,
    .set_char = (graphic_be_set_char_t)bcb0802_set_char,
    .set_line = (graphic_be_set_line_t)bcb0802_set_line,
    .set_screen = (graphic_be_set_screen_t)bcb0802_set_screen,
    .expose = (graphic_be_expose_t)bcb0802_expose,
    
    .loadextr = (graphic_be_loadextr_t)bcb0802_loadextr,
};

#define B7066_CHAR(__C__, __B3_0__, __B7_4__)   [(u8)__C__] = ((__B7_4__ << 4) | (__B3_0__ << 0))

static const
u8 char2value[256] = {
    [0 ... 255] = 0xf,
	B7066_CHAR(0, 0x0, 0x2),
    /* latin1 things in ascii sort */
    B7066_CHAR(' ', 0x0, 0x2), B7066_CHAR('!', 0x1, 0x2), B7066_CHAR('"', 0x2, 0x2), B7066_CHAR('#', 0x3, 0x2), B7066_CHAR('$', 0x4, 0x2), B7066_CHAR('%', 0x5, 0x2),
    B7066_CHAR('&', 0x6, 0x2), B7066_CHAR('\'', 0x7, 0x2), B7066_CHAR('(', 0x8, 0x2), B7066_CHAR(')', 0x9, 0x2), B7066_CHAR('*', 0xa, 0x2), B7066_CHAR('+', 0xb, 0x2),
    B7066_CHAR('-', 0xc, 0x2), B7066_CHAR(',', 0xd, 0x2), B7066_CHAR('.', 0xe, 0x2), B7066_CHAR('/', 0xf, 0x2),

    B7066_CHAR('0', 0x0, 0x3), B7066_CHAR('1', 0x1, 0x3), B7066_CHAR('2', 0x2, 0x3), B7066_CHAR('3', 0x3, 0x3), B7066_CHAR('4', 0x4, 0x3),
    B7066_CHAR('5', 0x5, 0x3), B7066_CHAR('6', 0x6, 0x3), B7066_CHAR('7', 0x7, 0x3), B7066_CHAR('8', 0x8, 0x3), B7066_CHAR('9', 0x9, 0x3),

    B7066_CHAR(':', 0xa, 0x3), B7066_CHAR(';', 0xb, 0x3), B7066_CHAR('=', 0xc, 0x3), B7066_CHAR('<', 0xd, 0x3), B7066_CHAR('>', 0xe, 0x3),
    B7066_CHAR('?', 0xf, 0x3), B7066_CHAR('@', 0x0, 0x4),

    B7066_CHAR('A', 0x1, 0x4), B7066_CHAR('B', 0x2, 0x4), B7066_CHAR('C', 0x3, 0x4), B7066_CHAR('D', 0x4, 0x4), B7066_CHAR('E', 0x5, 0x4),
    B7066_CHAR('F', 0x6, 0x4), B7066_CHAR('G', 0x7, 0x4), B7066_CHAR('H', 0x8, 0x4), B7066_CHAR('I', 0x9, 0x4), B7066_CHAR('J', 0xa, 0x4),
    B7066_CHAR('K', 0xb, 0x4), B7066_CHAR('L', 0xc, 0x4), B7066_CHAR('M', 0xd, 0x4), B7066_CHAR('N', 0xe, 0x4), B7066_CHAR('O', 0xf, 0x4),
    B7066_CHAR('P', 0x0, 0x5), B7066_CHAR('Q', 0x1, 0x5), B7066_CHAR('R', 0x2, 0x5), B7066_CHAR('S', 0x3, 0x5), B7066_CHAR('T', 0x4, 0x5),
    B7066_CHAR('U', 0x5, 0x5), B7066_CHAR('V', 0x6, 0x5), B7066_CHAR('W', 0x7, 0x5), B7066_CHAR('X', 0x8, 0x5), B7066_CHAR('Y', 0x9, 0x5), B7066_CHAR('Z', 0xa, 0x5),

    B7066_CHAR('[', 0xb, 0x5), B7066_CHAR('\\', 0xc, 0x5),B7066_CHAR(']', 0xd, 0x5), B7066_CHAR('^', 0xe, 0x5),B7066_CHAR('_', 0xf, 0x5), B7066_CHAR('`', 0x0, 0x6),

    B7066_CHAR('a', 0x1, 0x6), B7066_CHAR('b', 0x2, 0x6), B7066_CHAR('c', 0x3, 0x6), B7066_CHAR('d', 0x4, 0x6), B7066_CHAR('e', 0x5, 0x6),
    B7066_CHAR('f', 0x6, 0x6), B7066_CHAR('g', 0x7, 0x6), B7066_CHAR('h', 0x8, 0x6), B7066_CHAR('i', 0x9, 0x6), B7066_CHAR('j', 0xa, 0x6),
    B7066_CHAR('k', 0xb, 0x6), B7066_CHAR('l', 0xc, 0x6), B7066_CHAR('m', 0xd, 0x6), B7066_CHAR('n', 0xe, 0x6), B7066_CHAR('o', 0xf, 0x6),
    B7066_CHAR('p', 0x0, 0x7), B7066_CHAR('q', 0x1, 0x7), B7066_CHAR('r', 0x2, 0x7), B7066_CHAR('s', 0x3, 0x7), B7066_CHAR('t', 0x4, 0x7),
    B7066_CHAR('u', 0x5, 0x7), B7066_CHAR('v', 0x6, 0x7), B7066_CHAR('w', 0x7, 0x7), B7066_CHAR('x', 0x8, 0x7), B7066_CHAR('y', 0x9, 0x7), B7066_CHAR('z', 0xa, 0x7),

    //B7066_CHAR('{', 0x0, 0x0), B7066_CHAR('|', 0x0, 0x0), B7066_CHAR('}', 0x0, 0x0), B7066_CHAR('~', 0x0, 0x0), 

    
    /* koi8r things in alphabet sort */
    B7066_CHAR('á', 0x1, 0x4), B7066_CHAR('â', 0x0, 0xa), B7066_CHAR('÷', 0x2, 0x4), B7066_CHAR('ç', 0x1, 0xa), B7066_CHAR('ä', 0x0, 0xe),
    B7066_CHAR('å', 0x5, 0x4), B7066_CHAR('³', 0x2, 0xa), B7066_CHAR('ö', 0x3, 0xa), B7066_CHAR('ú', 0x4, 0xa), B7066_CHAR('é', 0x5, 0xa),
    B7066_CHAR('ê', 0x6, 0xa), B7066_CHAR('ë', 0xb, 0x4), B7066_CHAR('ì', 0x7, 0xa), B7066_CHAR('í', 0xd, 0x4), B7066_CHAR('î', 0x8, 0x4),
    B7066_CHAR('ï', 0xf, 0x4), B7066_CHAR('ð', 0x8, 0xa), B7066_CHAR('ò', 0x0, 0x5), B7066_CHAR('ó', 0x3, 0x4), B7066_CHAR('ô', 0x4, 0x5),
    B7066_CHAR('õ', 0x9, 0xa), B7066_CHAR('æ', 0xa, 0xa), B7066_CHAR('è', 0x8, 0x5), B7066_CHAR('ã', 0x1, 0xe), B7066_CHAR('þ', 0xb, 0xa),
    B7066_CHAR('û', 0xc, 0xa), B7066_CHAR('ý', 0x2, 0xe), B7066_CHAR('ÿ', 0xd, 0xa), B7066_CHAR('ù', 0xe, 0xa), /* B7066_CHAR('ø', 0x0, 0x0), */
    B7066_CHAR('ü', 0xf, 0xa), B7066_CHAR('à', 0x0, 0xb), B7066_CHAR('ñ', 0x1, 0xb),
    
    
    B7066_CHAR('Á', 0x1, 0x6), B7066_CHAR('Â', 0x2, 0xb), B7066_CHAR('×', 0x3, 0xb), B7066_CHAR('Ç', 0x4, 0xb), B7066_CHAR('Ä', 0x3, 0xe),
    B7066_CHAR('Å', 0x5, 0x6), B7066_CHAR('£', 0x5, 0xb), B7066_CHAR('Ö', 0x6, 0xb), B7066_CHAR('Ú', 0x7, 0xb), B7066_CHAR('É', 0x8, 0xb),
    B7066_CHAR('Ê', 0x9, 0xb), B7066_CHAR('Ë', 0xa, 0xb), B7066_CHAR('Ì', 0xb, 0xb), B7066_CHAR('Í', 0xc, 0xb), B7066_CHAR('Î', 0xd, 0xb),
    B7066_CHAR('Ï', 0xf, 0x6), B7066_CHAR('Ð', 0xe, 0xb), B7066_CHAR('Ò', 0x0, 0x7), B7066_CHAR('Ó', 0x3, 0x6), B7066_CHAR('Ô', 0xf, 0xb),
    B7066_CHAR('Õ', 0x9, 0x7), B7066_CHAR('Æ', 0x4, 0xe), B7066_CHAR('È', 0x8, 0x7), B7066_CHAR('Ã', 0x5, 0xe), B7066_CHAR('Þ', 0x0, 0xc),
    B7066_CHAR('Û', 0x1, 0xc), B7066_CHAR('Ý', 0x6, 0xd), B7066_CHAR('ß', 0x2, 0xc), B7066_CHAR('Ù', 0x3, 0xc), B7066_CHAR('Ø', 0x4, 0xc),
    B7066_CHAR('Ü', 0x5, 0xc), B7066_CHAR('À', 0x6, 0xc), B7066_CHAR('Ñ', 0x7, 0xc),

    /* bell */
    B7066_CHAR('\x09', 0xd, 0xe),
	
    /* extr chars */
    B7066_CHAR('\x01', 0x0, 0x0),
    B7066_CHAR('\x02', 0x1, 0x0),
    B7066_CHAR('\x03', 0x2, 0x0),
    B7066_CHAR('\x04', 0x3, 0x0),
    B7066_CHAR('\x05', 0x4, 0x0),
    B7066_CHAR('\x06', 0x5, 0x0),
    B7066_CHAR('\x07', 0x6, 0x0),
    B7066_CHAR('\x08', 0x7, 0x0),
};
