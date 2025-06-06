#define CMT2300A_MODULE
#include "cmt2300a_i.h"
#include "log.h"

#define CS_LOW()    DPORT_CLR(this->cs_port, this->cs_mask)
#define CS_HI()     DPORT_SET(this->cs_port, this->cs_mask)

#define POW_OFF()    DPORT_CLR(this->pow_port, this->pow_mask)
#define POW_ON()\
{\
	DPORT_SET(this->pow_port, this->pow_mask);\
	msleep(10);\
}

static 
b8 cmt2300a_transmit(THIS,
                     const u8 cnt,
                     const slice_t * slices,
                     const u8 repeat_cnt)
{
    #define CHEK_TRES() { if(false == res) { goto transmit_exit; } }
    b8 res = false;

	/* TODO: may be save CMT2300A_GPIO_NUM to this; this->gpio_num */
	const u8 pedal_tx[2] = {CUS_FIFO_CTL, CMT2300A_GPIO_CONF(1, CMT2300A_GPIO_NUM)};
	const u8 pedal_rx[2] = {CUS_FIFO_CTL, CMT2300A_GPIO_CONF(0, CMT2300A_GPIO_NUM)};

	spi_lock(this->spi);

	CS_LOW();
	res = spi_tx(this->spi, cmt2300a_goto_stby, sizeof(cmt2300a_goto_stby));
	CHEK_TRES();

	/* Setup to pedal RX GPIO2 */
	DPORT_DOUT_INIT(this->int_port, this->int_mask);

	res = spi_tx(this->spi, cmt2300a_goto_tx, sizeof(cmt2300a_goto_tx));
	CHEK_TRES();

	res = spi_tx(this->spi, pedal_tx, sizeof(pedal_tx));
	CS_HI();
	CHEK_TRES();

	spi_unlock(this->spi);

	for(uf8 count = repeat_cnt; count; --count){
		const slice_t * restrict src = slices;
		/* tx block */
		for(uf8 tx_count = 0; tx_count < cnt; ++tx_count, ++src){
			const uf32 len = src->length;
			const uf8 lvl = src->level;
			DPORT_WRITE(this->int_port, this->int_mask, lvl);
			/**
			 * very inaccurate timing
			 * but TIMER work very strange
			 */
			usleep(len);
		}
	}

	spi_lock(this->spi);

 transmit_exit:;

	CS_LOW();
	spi_tx(this->spi, cmt2300a_goto_stby, sizeof(cmt2300a_goto_stby));

	DPORT_DIN_INIT(this->int_port, this->int_mask);

	spi_tx(this->spi, pedal_rx, sizeof(pedal_rx));
	CS_HI();

	spi_unlock(this->spi);
    
    return res;
}

static 
s32 cmt2300a_receive(THIS,
					 slice_t * slices,
					 const u32 size,
					 const u32 msec)
{
    #define CHEK_RRES() { if(false == res) { goto receive_exit; } }
    s32 res = false;

	/* TODO: may be save CMT2300A_GPIO_NUM to this; this->gpio_num */
	const u8 pedal_rx[2] = {CUS_FIFO_CTL, CMT2300A_GPIO_CONF(0, CMT2300A_GPIO_NUM)};

	spi_lock(this->spi);

	CS_LOW();
	res = spi_tx(this->spi, cmt2300a_goto_stby, sizeof(cmt2300a_goto_stby));
	CHEK_RRES();

	/* Setup to pedal RX GPIO2 */
	DPORT_DIN_INIT(this->int_port, this->int_mask);

	/**
	 * start IC counting before cmt2300a RX
	 * for can set levels after work
	 */
	ic_lock(this->ic);
	/* prepare timer freq for 1us IC counting */
	ic_prep(this->ic, TPSET_IC_1US);
	/* start counting */
	ic_start_rx(this->ic, 0, (u16 * )slices, size);

	const b8 start_level = (0 < DPORT_GET(this->int_port, this->int_mask));

	res = spi_tx(this->spi, cmt2300a_goto_rx, sizeof(cmt2300a_goto_rx));
	CHEK_RRES();

	res = spi_tx(this->spi, pedal_rx, sizeof(pedal_rx));
	CS_HI();
	CHEK_RRES();

	spi_unlock(this->spi);

	/* wait msec or size limit */
	ic_wait(this->ic, msec);//may be just msleep(msec);???
	/* stop timer and get not filled cnt */
	const u32 ic_res = ic_stop(this->ic);
	ic_unlock(this->ic);

	const u32 cnt = size - ic_res;
	/* ic values need to process */
	res = ic_process((u16 * )slices, cnt, 0x7fff);
	/**
	 * set levels after work
	 */
	b8 level = start_level;
	for(u32 i = 0; i < res; i++){
		slices[i].level = level;
		level = !level;
	}

	spi_lock(this->spi);

receive_exit:;

	CS_LOW();
	spi_tx(this->spi, cmt2300a_goto_stby, sizeof(cmt2300a_goto_stby));
	DPORT_DIN_INIT(this->int_port, this->int_mask);
	spi_tx(this->spi, pedal_rx, sizeof(pedal_rx));
	CS_HI();

	spi_unlock(this->spi);
    
    return res;
}

/**
 * turn device to sleep mode
 * @param be		- backend
 * @return		- true if success
 */
static
b8 cmt2300a_sleep(THIS)
{
    spi_lock(this->spi);

    CS_LOW();
    const b8 res = spi_tx(this->spi, cmt2300a_goto_sleep, sizeof(cmt2300a_goto_sleep));
    CS_HI();

    spi_unlock(this->spi);
    return res;
}

/**
 * wake device from sleep
 * @param be		- backend
 * @return		- true if success
 */
static
b8 cmt2300a_wake(THIS)
{
    spi_lock(this->spi);

    CS_LOW();
    const b8 res = spi_tx(this->spi, cmt2300a_goto_stby, sizeof(cmt2300a_goto_stby));
    CS_HI();

    spi_unlock(this->spi);
    return res;
}

/**
 * turn device off
 * @param be		- backend
 * @return		- true if success
 */
static
b8 cmt2300a_off(THIS)
{
    const b8 result = cmt2300a_sleep(this);
    msleep(5);
    POW_OFF();
    return result;
}

/**
 * turn device on
 * @param be		- backend
 * @return		- true if success
 */
static
b8 cmt2300a_on(THIS)
{
    POW_ON();
    msleep(5);
    return cmt2300a_wake(this);
}

/**
 * set central modulation freq
 * @param be		- backend
 * @param freq		- freq identifier
 * @return 		- true if success
 */
// static
// b8 cmt2300a_freq(THIS, const u32 freq)
// {
//     return false;
// }

static
void cmt2300a_init_regs(THIS){
    spi_lock(this->spi);

    CS_LOW();
    spi_tx(this->spi, cmt2300a_config, sizeof(cmt2300a_config));

    msleep(1);

    spi_tx(this->spi, cmt2300a_goto_sleep, sizeof(cmt2300a_goto_sleep));
    CS_HI();

    spi_unlock(this->spi);
}


ll_be_t* cmt2300a_init(THIS, const cmt2300a_conf_t * const conf)
{
    memclr(this, sizeof(cmt2300a_t));
    this->ll_be.lft = &cmt2300a_ft;
    this->ll_be.this = this;
    
    this->spi = conf->spi;
	this->ic = conf->ic;
	this->pwm = conf->pwm;

    this->int_port = conf->int_port;
    this->cs_port = conf->cs_port;
    this->pow_port = conf->pow_port;

    this->int_mask = (1 << conf->int_pin);
    this->cs_mask = (1 << conf->cs_pin);
    this->pow_mask = (1 << conf->pow_pin);

    //this->int_pin = conf->int_pin;

    DPORT_DOUT_INIT(this->cs_port, this->cs_mask);
    DPORT_DOUT_INIT(this->pow_port, this->pow_mask);
    DPORT_DIN_INIT(this->int_port, this->int_mask);

    // const exti_group_t group = extint_configure(this->int_port, this->int_pin, EXTI_Trigger_Rising_Falling);
    // this->exti = group;

    CS_HI();
    POW_ON();
    
    /* cmt2300a registers init action */
    cmt2300a_init_regs(this);

    return &this->ll_be;
}
