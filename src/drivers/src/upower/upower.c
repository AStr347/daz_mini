#define UPOWER_MODULE
#include "upower.h"

#define UPOWER_TIMEOUT_MS   (30000)

const
upower_levels_t upower_pos2level[UPOWERL_LEVEL_COUNT] = {
    [UPOWERL_NEED_OFF_P] = UPOWERL_NEED_OFF,
    [UPOWERL_LOW_P] = UPOWERL_LOW,
    [UPOWERL_NORMAL_P] = UPOWERL_NORMAL,
    [UPOWERL_PREFULL_P] = UPOWERL_PREFULL,
    [UPOWERL_FULL_P] = UPOWERL_FULL,
};

upower_levels_t upower_charge(upower_t * const this){
	return this->be->charge;
}

upower_stat_t upower_stat(upower_t * const this){
	return this->be->stat;
}

b8 upower_is_good(upower_t * const this){
    const upower_stat_t stat = this->be->stat;
    if(UPOWERS_CHARGING == stat || UPOWERS_CHARGED == stat){
        return true;
    }
    const upower_levels_t level = this->be->charge;
    return (UPOWERL_NEED_OFF != level);
}

void upower_put_com(upower_t * const this, const upower_com_t com){
    mlock(this->mutex);

    const b8 res = sfifo8_put(&this->fifo, &com, 1);
    if(res){
        csignal(this->cond);
    }

    mulock(this->mutex);
}

static
void upower_routine(upower_t * const this){
    const upower_conf_t * const conf = this->conf;
    this->be = conf->be_init(conf->be, conf->be_conf);

    __unused
    u32 waited = 0;

    mlock(this->mutex);
    for(;;){
        const b8 is_empty = sfifo8_is_empty(&this->fifo);
        if(false == is_empty){
            upower_com_t new_com = UPOWERC_NOOP;
            const u16 geted = sfifo8_get(&this->fifo, &new_com, 1);
            if(0 == geted){
                sfifo8_reset(&this->fifo);
                continue;
            }
            switch(new_com){
                case UPOWERC_NOOP:{ this->mode = UPOWERM_NONE; }break;
                case UPOWERC_MONITORING:{ this->mode = UPOWERM_MONITORING; }break;
                case UPOWERC_POWERUNLOCK:{
                    this->mode = UPOWERM_NONE;
                    this_punlock();
                }break;
                case UPOWERC_POWERLOCK:{
                    this->mode = UPOWERM_MONITORING;
                    this_plock();
                }break;
                default:{
                    this->mode = UPOWERM_NONE;
                }break;
            }
            /**
             * goto next circle for do all commands
             * before routine
             */
            continue;
        }

        switch (this->mode) {
            case UPOWERM_NONE: {
                cmtwait(this->cond, this->mutex, UPOWER_TIMEOUT_MS, waited);
            } break;
            case UPOWERM_MONITORING: {
                const upower_stat_t stat = this->be->stat;
                this_monitor();
                
                if (unlikely(stat != this->be->stat)){
                    inevent_event_arg1(this->input, EVENT_POWER_U, stat);
                }
                cmtwait(this->cond, this->mutex, UPOWER_TIMEOUT_MS, waited);
            } break;
            default:{
                this->mode = UPOWERM_NONE;
            }break;
        }
    }
}

void upower_init(upower_t * const this, const upower_conf_t * const conf){
    memclr(this, sizeof(upower_t));

    minit(this->mutex);
    cinit(this->cond);
    sfifo8_init(&this->fifo, this->combuff, UPOWER_COMBUFF_SIZE);

    this->mode = UPOWERM_NONE;
    this->conf = conf;
    this->input = conf->input;

    TaskCreate(this, upower_routine, UPOWER_STACK_SIZE, UPOWER_PRIORITY);
}
