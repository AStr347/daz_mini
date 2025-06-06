#define SSIGN_MODULE
#include "ssign_i.h"

static
b8 ssign_proc(ssign_t * const this){
    const ssign_mode_t mode = this->proc.curr_mode;
    if(SMODE_NONE == mode){
        return true;
    }

    u8 pos = this->proc.pos;
    const ssign_pattern_t * const pattern = this->proc.pattern;
    if(pattern->len == pos){
        return true;
    }

    const b8 level = pattern->slice[pos].level;
    const u16 ms_time = pattern->slice[pos].length;

    if(0 != (mode & SMODE_FLASH)){
        DPORT_WRITE(this->l_port, this->l_mask, level);
    }
    if(0 != (mode & SMODE_MOTOR)){
        DPORT_WRITE(this->m_port, this->m_mask, level);
    }
    msleep(ms_time);

    this->proc.pos = pos + 1;
    return false;
}

static
void ssign_routine(ssign_t * const this){
    mlock(this->mutex);
    for(;;){
        const b8 is_empty = sfifo8_is_empty(&this->com);
        if(false == is_empty){
            ssign_com_t new_com = SCOM_NONE;
            const u8 geted = sfifo8_get(&this->com, &new_com, 1);
            if(1 == geted){
                switch (new_com) {
                    case SCOM_NONE:{
                        this->proc.curr_com = SCOM_NONE;
                    } break;
                    case SCOM_IND:{
                        u8 mode_ind[2];
                        const u8 args_geted = sfifo8_get(&this->com, mode_ind, 2);
                        const ssign_mode_t mode = mode_ind[0];
                        const alarm_description_t ind = mode_ind[1];

                        if(2 == args_geted && ((ind < ALD_COUNT) && (mode <= SMODE_FULL))){
                            this->proc.curr_com = new_com;
                            this->proc.curr_mode = mode;
                            this->proc.pos = 0;
                            this->proc.pattern = ssign_patterns[ind];
                        } else {
                            this->proc.curr_com = SCOM_NONE;
                            continue;
                        }
                    } break;
                }
            }
        }
        switch (this->proc.curr_com) {
            case SCOM_NONE: {
                this->proc.curr_com = SCOM_NONE;
                this->proc.curr_mode = SMODE_FULL;
                this->proc.pos = 0;
                this->proc.pattern = NULL;
                DPORT_CLR(this->l_port, this->l_mask);
                DPORT_CLR(this->m_port, this->m_mask);

                __unused
                u32 waited = 0;
                cmwait(this->cond, this->mutex, waited);
            } break;

            case SCOM_IND: {
                mulock(this->mutex);
                const b8 proc_end = ssign_proc(this);
                mlock(this->mutex);
                if(true == proc_end){
                    this->proc.curr_com = SCOM_NONE;
                }
            } break;
        }
    }
}

void ssign_init(ssign_t * const this, const ssign_conf_t * const conf){
    memclr(this, sizeof(ssign_t));

    this->l_port = conf->l_port;
    this->l_mask = (1 << conf->l_pin);
    this->m_port = conf->m_port;
    this->m_mask = (1 << conf->m_pin);

    DPORT_DOUT_INIT(this->l_port, this->l_mask);
    DPORT_DOUT_INIT(this->m_port, this->m_mask);

    DPORT_CLR(this->l_port, this->l_mask);
    DPORT_CLR(this->m_port, this->m_mask);

    cinit(this->cond);
    minit(this->mutex);

    sfifo8_init(&this->com, this->combuff, SSIGN_COM_BUFF);

    this->proc.curr_com = SCOM_NONE;
    this->proc.curr_mode = SMODE_FULL;
    this->proc.pos = 0;
    this->proc.pattern = NULL;

    TaskCreate(this, ssign_routine, SSIGN_STACK_SIZE, SSIGN_PRIORITY);
}

static
void ssign_com(ssign_t * const this, const u8 * const com, const u8 siz){
    mlock(this->mutex);

    const b8 res = sfifo8_put(&this->com, com , siz);
    if(res){
    	csignal(this->cond);
    }
    mulock(this->mutex);
}

void ssign_stop(ssign_t * const this){
    const u8 dtw[] = {
        [0] = SCOM_NONE,
    };
    ssign_com(this, dtw, sizeof(dtw));
}

void ssign_set(ssign_t * const this, const ssign_mode_t mode, const alarm_description_t ind){
    const u8 dtw[] = {
        [0] = SCOM_IND,
        [1] = mode,
        [2] = ind,
    };
    ssign_com(this, dtw, sizeof(dtw));
}
