#define BCB0802_MODULE
#include "bcb0802_i.h"
#include "extsymbols_i.h"

/**
 * set pins to transmit 1 raw data
 */
static __noinline
void bcb0802_set_data(THIS, const u8 val){
	if(false == this->powered){
		return;
	}

	DPORT_CLR(this->e_port, this->e_mask);
	for(u8 pos = 0; pos < 8; pos++){
		const u8 mask = (1 << pos);
		const u8 level = (0 < (val & mask));
		DPORT_WRITE(this->d_ports[pos], this->d_masks[pos], level);
	}
    DPORT_SET(this->e_port, this->e_mask);
    usleep(40);
    DPORT_CLR(this->e_port, this->e_mask);
}

/**
 * write onec ST7066 com
 */
static __noinline
void bcb0802_com_write(THIS, const u8 com){
	if(false == this->powered){
		return;
	}

    DPORT_CLR(this->rs_port, this->rs_mask);
    DPORT_CLR(this->rw_port, this->rw_mask);
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    bcb0802_set_data(this, com);
    
    DPORT_SET(this->rs_port, this->rs_mask);
    DPORT_SET(this->rw_port, this->rw_mask);
}

/**
 * write many ST7066 coms
 * com0, com1, com2... comN
 */
static __noinline
void bcb0802_coms_write(THIS, const u8 * data, const u8 siz){
	if((0 == siz) || (false == this->powered)){
		return;
	}

    DPORT_CLR(this->rs_port, this->rs_mask);
    DPORT_CLR(this->rw_port, this->rw_mask);
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    const u8 * src = data;

    for(u8 i = 0; i < siz; i++, src++){
        bcb0802_set_data(this, *src);
    }
    
    DPORT_SET(this->rs_port, this->rs_mask);
    DPORT_SET(this->rw_port, this->rw_mask);
}

/**
 * write once ST7066 ddram data
 */
static __noinline
void bcb0802_data_write(THIS, const u8 addres, const u8 data){
	if(false == this->powered){
		return;
	}

    bcb0802_com_write(this, addres);

    DPORT_SET(this->rs_port, this->rs_mask);
    DPORT_CLR(this->rw_port, this->rw_mask);
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    bcb0802_set_data(this, data);
    
    DPORT_SET(this->rs_port, this->rs_mask);
    DPORT_SET(this->rw_port, this->rw_mask);
}

/**
 * write once ST7066 ddram data
 */
static __noinline
void bcb0802_double_data_write(THIS,
                               const u8 addres,
                               const u8 data0,
                               const u8 data1)
{
	if(false == this->powered){
		return;
	}

    bcb0802_com_write(this, addres);

    DPORT_SET(this->rs_port, this->rs_mask);
    DPORT_CLR(this->rw_port, this->rw_mask);
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    bcb0802_set_data(this, data0);
    bcb0802_set_data(this, data1);
    
    DPORT_SET(this->rs_port, this->rs_mask);
    DPORT_SET(this->rw_port, this->rw_mask);
}

/**
 * write ddram_addr0, value0, ddram_addr1, value1... ddram_addrN, valueN
 */
static __noinline
void bcb0802_datas_write(THIS, const u8 * data, const u8 siz){
    if((0 != (siz & 1)) || (false == this->powered)){
        /* we wait pair in data, addr + raw */
        return;
    }
    const u8 * src = data;

    for(u8 i = 0; i < siz; i+=2){
        const u8 addr = *src++;
        const u8 data = *src++;

        bcb0802_data_write(this, addr, data);
    }
}

static __noinline
void bcb0802_display_init(THIS){
    static const
    u8 dtw[] = {
        [0] = (ST7066_INIT | ST7066_INIT_8BIT | ST7066_INIT_2LINE | ST7066_INIT_5X8FONT),
        [1] = (ST7066_DISP | ST7066_ON),
        [2] = (ST7066_CLEAR),
        [3] = (ST7066_ENTRY | ST7066_ENTRY_INC),
    };
    bcb0802_coms_write(this, dtw, sizeof(dtw));
}

/* backend iface */

static
void bcb0802_off(THIS)
{
    DPORT_CLR(this->power_port, this->power_mask);
    this->powered = false;
    msleep(20);

    /**
     * screen can eat power throw control pins
     * do them DIN
     */
    DPORT_DIN_INIT(this->rs_port, this->rs_mask);
    DPORT_DIN_INIT(this->rw_port, this->rw_mask);
    DPORT_DIN_INIT(this->e_port, this->e_mask);
    for(u8 pos = 0; pos < 8; pos++){
        DPORT_DIN_INIT(this->d_ports[pos], this->d_masks[pos]);
    }
}

static
void bcb0802_on(THIS)
{
	/**
	 * screen can eat power throw control pins
	 * restore them DOUT
	 */
    DPORT_DOUT_INIT(this->rs_port, this->rs_mask);
    DPORT_SET(this->rs_port, this->rs_mask);
    DPORT_DOUT_INIT(this->rw_port, this->rw_mask);
    DPORT_SET(this->rw_port, this->rw_mask);
    DPORT_DOUT_INIT(this->e_port, this->e_mask);
    DPORT_CLR(this->e_port, this->e_mask);
    for(u8 pos = 0; pos < 8; pos++){
        DPORT_DOUT_INIT(this->d_ports[pos], this->d_masks[pos]);
        DPORT_CLR(this->d_ports[pos], this->d_masks[pos]);
    }

    DPORT_SET(this->power_port, this->power_mask);
    this->powered = true;
    msleep(50);
    bcb0802_display_init(this);
    msleep(20);
}

static
void bcb0802_clear(THIS)
{
	memclr(this->screen, GRAPHIC_BE_ROW_CNT * GRAPHIC_BE_COLUMN_CNT);
    for(u8 j = 0; j < GRAPHIC_BE_ROW_CNT; j++){
        this->expose[j] = 0xff;
    }

    bcb0802_com_write(this, ST7066_CLEAR);
    msleep(20);
}

static
void bcb0802_set_char(THIS,
                      const u8 c,
                      const u8 x,
                      const u8 y)
{
    if(GRAPHIC_BE_COLUMN_CNT <= x || GRAPHIC_BE_ROW_CNT <= y){
        return;
    }
    const u8 c_now = this->screen[y][x];
    if(c_now != c){
        this->screen[y][x] = c;
        this->expose[y] |= (1 << x);
    }
}

static
void bcb0802_set_line(THIS,
                      const u8 * line,
                      const u8 len,
                      const u8 y)
{
    if(GRAPHIC_BE_COLUMN_CNT < len || GRAPHIC_BE_ROW_CNT <= y){
        return;
    }
    for(u8 i = 0; i < len; i++){
        const u8 c = line[i];
        const u8 c_now = this->screen[y][i];
        if(c_now != c){
            this->screen[y][i] = c;
            this->expose[y] |= (1 << i);
        }
    }
}

static
void bcb0802_set_screen(THIS,
                        const u8 ** lines,
                        const u8 * lens,
                        const u8 lines_cnt)
{
    if(GRAPHIC_BE_ROW_CNT < lines_cnt){
        return;
    }
    for(u8 j = 0; j < lines_cnt; j++){
        const u8 may_len = lens[j];
        const u8 len = GRAPHIC_BE_COLUMN_CNT < may_len? GRAPHIC_BE_COLUMN_CNT : may_len;
        for(u8 i = 0; i < len; i++){
            const u8 c = lines[j][i];
            const u8 c_now = this->screen[j][i];
            if(c_now != c){
                this->screen[j][i] = c;
                this->expose[j] |= (1 << i);
            }
        }
    }
}

static
void bcb0802_expose(THIS){
	if(false == this->powered){
		return;
	}

    u8 dtw[(GRAPHIC_BE_COLUMN_CNT * GRAPHIC_BE_ROW_CNT * 2) + 2];
    memclr(dtw, sizeof(dtw));

    u8 * dst = dtw;
    u16 siz = 0;
    for(u8 j = 0; j < GRAPHIC_BE_ROW_CNT; j++){
    	if(0 == this->expose[j]){
    		continue;
    	}
        for(u8 i = 0; i < GRAPHIC_BE_COLUMN_CNT; i++){
            const b8 need_expose = (0 < (this->expose[j] & (1 << i)));
            if(false == need_expose){
            	continue;
            }
			const u8 c = this->screen[j][i];
			*dst++ = (ST7066_DDRAM | (j << ST7066_ROW_P) | i);
			*dst++ = char2value[c];
			siz += 2;
        }
        this->expose[j] = 0;
    }
    bcb0802_datas_write(this, dtw, siz);
}

/**
 * load extar char to selected pos
 */
static
void bcb0802_loadextr(THIS, const u8 pos, const bcb0802_extsymb_t ext){
    if(false == this->powered){
		return;
	}
    if('\x09' <= pos || BCB0802_EXTS_CNT <= ext){
        return;
    }

    const u8 real_pos = (pos - 1) * 8;//char2value[pos];

    const u8 * const extdata = __extsymbs[ext];
    const u8 * src = extdata;
    
    for(u8 i = 0; i < EXTSYMBIL_LEN; i++){
        const u8 data0 = (ST7066_CGRAM | (real_pos + i));
        const u8 data1 = *src++;

        bcb0802_data_write(this, data0, data1);
    }
}

graphic_be_t * bcb0802_init(THIS, const bcb0802_conf_t * const conf){
    memclr(this, sizeof(bcb0802_t));
    
    /* graphic backend init */
    this->be.rowcnt = GRAPHIC_BE_ROW_CNT;
    this->be.columncnt = GRAPHIC_BE_COLUMN_CNT;
    this->be.ft = &bcb0802_ft;
    this->be.this = this;

    /* ports/pins init */
    this->rs_port = conf->rs_port;
    this->rs_mask = (1 << conf->rs_pin);
    DPORT_DOUT_INIT(this->rs_port, this->rs_mask);
    DPORT_SET(this->rs_port, this->rs_mask);

    this->rw_port = conf->rw_port;
    this->rw_mask = (1 << conf->rw_pin);
    DPORT_DOUT_INIT(this->rw_port, this->rw_mask);
    DPORT_SET(this->rw_port, this->rw_mask);

    this->e_port = conf->e_port;
    this->e_mask = (1 << conf->e_pin);
    DPORT_DOUT_INIT(this->e_port, this->e_mask);
    DPORT_CLR(this->e_port, this->e_mask);


    this->power_port = conf->power_port;
    this->power_mask = (1 << conf->power_pin);
    DPORT_DOUT_INIT(this->power_port, this->power_mask);
    DPORT_CLR(this->power_port, this->power_mask);
    this->powered = false;

    for(u8 i = 0; i < 8; i++){
        this->d_ports[i] = conf->d_ports[i];
        this->d_masks[i] = (1 << conf->d_pins[i]);
        DPORT_DOUT_INIT(this->d_ports[i], this->d_masks[i]);
        DPORT_CLR(this->d_ports[i], this->d_masks[i]);
    }

    /* diplay off */
    bcb0802_off(this);
    return &this->be;
}
