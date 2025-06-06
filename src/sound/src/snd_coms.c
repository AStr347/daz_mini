#define SND_MODULE
#define SND_STATIC_DATA
#include "snd_engine_i.h"

/* all tasks, that should do,
 * when sound engine is stopped
 * @param stat	- [in] current stat
 */
static
void snd_stop_tasks(THIS, const u8 stat) {
    if(SND_STAT_COUNT > stat){
        snd_stops[stat](this);
    }
    snd_clear(this);
}

static
void snd_coms_noop(THIS) {
    /* wrong command - reset command buffer */
    sfifo8_reset(&this->combuf);
};

static
void snd_coms_play(THIS) {
    const uf stat = this->stat;
    snd_stop_tasks(this, stat);
    u8 get_result[5];

    const uf8 read_count = sfifo8_get(&this->combuf, get_result, 5);
    if (5 != read_count) {
        sfifo8_reset(&this->combuf);
        this->stat = SND_STAT_IDLE;
        return;
    }
    /* ok, we take command */
    u32 sound_raw = get_result[0] << 24;
    sound_raw |= get_result[1] << 16;
    sound_raw |= get_result[2] << 8;
    sound_raw |= get_result[3] << 0;
    this->rwa.sound = (const sound_t*) sound_raw;
    this->rwa.play_cnt = get_result[4];

    const sound_t * restrict sound = this->rwa.sound;
    const u32 codec = sound->codec;
    if(SND_CODEC_CNT <= codec){
        this->stat = SND_STAT_IDLE;
        return;
    }
    const gen_ft_t * genft = generator_list[codec];
    if(NULL == genft){
        this->stat = SND_STAT_IDLE;
        return;
    }
    this->codec = codec;
    this->gen.ft = genft;

    memclr(&this->gen.pbuf, sizeof(this->gen.pbuf));
    const b8 gen_init_res = this_gen_init(sound);
    if(false == gen_init_res){
        this->stat = SND_STAT_IDLE;
        return;
    }

    b8 have_data = this_gen_fill();
    this_vol_set(this->volume, &this->gen.pbuf);
    this->gen.pbuf.bufsel ^= 0x01;

    have_data = this_gen_fill();
    this_vol_set(this->volume, &this->gen.pbuf);
    this->gen.pbuf.bufsel ^= 0x01;

    const b8 be_on_res = this_be_on();
    if(false == be_on_res){
        this_gen_stop();
        this->stat = SND_STAT_IDLE;
        return;
    }

    this_be_start(&this->gen.pbuf);
    this->stat = SND_STAT_PLAYING;
    this->rwa.have_data = have_data;

    mulock(this->mutex);
	this_be_wait();
	mlock(this->mutex);
};

static
void snd_coms_stop(THIS) {
    const uf8 stat = this->stat;
    snd_stop_tasks(this, stat);
    this->stat = SND_STAT_IDLE;
};

static
void snd_coms_volume(THIS) {
    u8 get_result = 0;
    const uf8 read_count = sfifo8_get(&this->combuf, &get_result, 1);
    if (1 != read_count) {
        sfifo8_reset(&this->combuf);
        return;
    }
    /* ok, we take command */
    this->volume = get_result;
};

const snd_coms_handlers_t snd_coms_handlers[SND_COM_COUNT] = {
    [SND_NOOP] = (snd_coms_handlers_t) snd_coms_noop,
    [SND_PLAY] = (snd_coms_handlers_t) snd_coms_play,
    [SND_STOP] = (snd_coms_handlers_t) snd_coms_stop,
    [SND_VOLUME] = (snd_coms_handlers_t) snd_coms_volume,
};
