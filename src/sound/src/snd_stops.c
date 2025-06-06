#define SND_MODULE
#include "snd_engine_i.h"

static
void save_off(THIS){
    this_be_stop();
    this_be_off();
}

static void snd_playing_stop(THIS)
{
    save_off(this);
    this_gen_stop();
    this->stat = SND_STAT_IDLE;
}

static
void snd_idle_stop(THIS)
{
    /* nothing */
}

const snd_stops_t snd_stops[SND_STAT_COUNT] = {
    [SND_STAT_IDLE] = (snd_stat_handler_t) snd_idle_stop,
	[SND_STAT_PLAYING] = (snd_stat_handler_t) snd_playing_stop,
};
