#define UPOWER_CHIP_MODULE
#include "upower_chip_i.h"

#include "log.h"

/**
 * adc levels for convert to upower_levels pos
 */
static const
u16 upower_chip_levels[UPOWERL_LEVEL_COUNT] = {
    [UPOWERL_NEED_OFF_P] = UPOWER_CHIP_NEED_OFF_ADC,
    [UPOWERL_LOW_P] = UPOWER_CHIP_LOW_ADC,
    [UPOWERL_NORMAL_P] = UPOWER_CHIP_NORMAL_ADC,
    [UPOWERL_PREFULL_P] = UPOWER_CHIP_PREFULL_ADC,
    [UPOWERL_FULL_P] = UPOWER_CHIP_FULL_ADC,
};

static __noinline
void upower_chip_update_charge(upower_chip_t* this){
    const u16 adc_value = adc_get_volt(this->adc_ch);
    u8 charge_pos = UPOWERL_LEVEL_COUNT;
    for (u8 i = 0; i < UPOWERL_LEVEL_COUNT; i++){
        if(adc_value <= upower_chip_levels[i]){
            charge_pos = i;
            break;
        }
    }
    
    LOGI("adc_volt_value: %d \t charge_pos: %d", adc_value, charge_pos);

    u8 level = 0;
    if(charge_pos == UPOWERL_LEVEL_COUNT){
        level = UPOWERL_FULL;
    } else {
        level = upower_pos2level[charge_pos];
    }
    this->be.charge = level;
}



/**
 * get stat value from charge chip pins
 * @return 
 */
static __noinline
void upower_chip_update_stat(upower_chip_t * const this){
    const b8 chg_stat = (0 < DPORT_GET(this->chg_port, this->chg_mask));
    const b8 std_stat = (0 < DPORT_GET(this->stat_port, this->stat_mask));
    const upower_stat_t stat = (chg_stat << 1) | (std_stat);

    this->be.stat = stat;
}

/* internal func */
static
void upower_chip_monitor(void * this){
    upower_chip_update_stat(this);
    upower_chip_update_charge(this);
}
static
void upower_chip_plock(void * this){
    /* nothing to do */
}
static
void upower_chip_punlock(void * this){
    /* nothing to do */
}

upower_be_t * upower_chip_init(upower_chip_t * const this, const upower_chip_conf_t * const conf){
    memclr(this, sizeof(upower_chip_t));

    this->be.ft = &upower_chip_ft;
    this->be.this = this;
    this->be.charge = UPOWERL_FULL;
    this->be.stat = UPOWERS_OFF;

    this->chg_port = conf->chg_port;
    this->stat_port = conf->stat_port;

    this->chg_mask = (1 << conf->chg_pin);
    this->stat_mask = (1 << conf->stat_pin);

    DPORT adc_port = conf->adc_port;
    const u16 adc_mask = (1 << conf->adc_pin);
    this->adc_ch = conf->adc_ch;

    DPORT_DIN_INIT_WPU(this->chg_port, this->chg_mask);
    DPORT_DIN_INIT_WPU(this->stat_port, this->stat_mask);
    DPORT_AN_INIT(adc_port, adc_mask);

    return (upower_be_t *)&this->be;
}
