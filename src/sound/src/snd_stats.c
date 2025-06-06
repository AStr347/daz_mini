#define SND_MODULE
#include "snd_engine_i.h"

typedef enum __packed {
    SND_PS_HAVE_DATA_P = 0,
    SND_PS_PLAY_CNT_P = 1,
    SND_PS_HAVE_DATA_M = (1 << SND_PS_HAVE_DATA_P),
    SND_PS_PLAY_CNT_M = (1 << SND_PS_PLAY_CNT_P),

    SND_PS_DONE = 0,
    SND_PS_LAST_CHUNK = SND_PS_HAVE_DATA_M,
    SND_PS_NEW_CHUNK = SND_PS_PLAY_CNT_M,
    SND_PS_PLAYING = SND_PS_PLAY_CNT_M | SND_PS_HAVE_DATA_M,
} play_states_t;

/**
 * @brief play handler
 * @param snd   - [in] sound handler
 */
static
void snd_playing_stat(THIS)
{
    b8 have_data = this->rwa.have_data;
    u8 play_cnt = this->rwa.play_cnt;
    const sound_t * restrict const sound = this->rwa.sound;
    samplebuffer_t * pbuf = &this->gen.pbuf;
    
    /* todo: tail of filled byffers should be filled with zeros */
    b8 need_wait = false;
    b8 done = false;

    const play_states_t play_state = (0 < play_cnt) | (0 < have_data);
    switch (play_state){
        case SND_PS_DONE:{
            have_data = 0;
            play_cnt = 0;
            pbuf->count[pbuf->bufsel] = 0;
            done = true;
        }break;
        case SND_PS_NEW_CHUNK:{
            const b8 gen_init_res = this_gen_init(sound);
            if(false == gen_init_res){
                have_data = 0;
                play_cnt = 0;
                pbuf->count[pbuf->bufsel] = 0;
                done = true;
            }
            have_data = this_gen_fill();
            --play_cnt;
        }break;

        case SND_PS_LAST_CHUNK:
        case SND_PS_PLAYING:{
            have_data = this_gen_fill();
            if(false == have_data){
            	--play_cnt;
            }
        }break;
    }

    /* play, have something to play */
    const b8 need_play = (0 != pbuf->count[pbuf->bufsel]);
    if(likely(need_play)){
        this_vol_set(this->volume, &this->gen.pbuf);
        pbuf->bufsel ^= 0x01;
        need_wait = true;
    }

    /* wait proc buffer of just wait */
    if(need_wait){
        /* todo: seems, this should also waked by commands */
        mulock(this->mutex);
        this_be_wait();
        mlock(this->mutex);
    }

    this->rwa.have_data = have_data;
    this->rwa.play_cnt = play_cnt;

    if(unlikely(done)){
        this->stat = SND_STAT_IDLE;
        this_be_wait();
        this_be_stop();
        this_be_off();
        snd_clear(this);
        /* playing done - proc input */
        inevent_event(this->input, EVENT_SOUND_PLAY_END);
    }
}

static
void snd_idle_stat(THIS){
    __unused
    u8 bogus = 0;
    cmwait(this->cond, this->mutex, bogus);
}


const snd_stat_handler_t snd_stat_handlers[SND_STAT_COUNT] = {
    [SND_STAT_IDLE] = (snd_stat_handler_t) snd_idle_stat,
	[SND_STAT_PLAYING] = (snd_stat_handler_t) snd_playing_stat,
};

