#define SND_MODULE
#include "snd_engine_i.h"
#include "board.h"

/**
 * send command to sound routine and wake the routine, if need
 * 
 * @param this	- [in] sound instance
 * @param com	- [in] command
 * @param len	- [in] command length
 */
static
b8 snd_com(THIS, const u8* com, const u8 len)
{
	mlock(this->mutex);
	const b8 result = sfifo8_put(&this->combuf, com, len);
	if(likely(result)){
		csignal(this->cond);
	}
	mulock(this->mutex);
	return result;
}

/**
 * proceed all sound tasks
 * @param snd		- sound device instance
 */
static
void snd_routine(THIS)
{
	mlock(this->mutex);
	const snd_conf_t * restrict const conf = this->conf;
	this->be = conf->be_init(conf->be, conf->be_conf);
	this->vol = conf->snd_vol_init(conf->snd_vol, conf->snd_vol_conf);
	
	for(;;) {
		/* command interpreter */
        u8 com = SND_NOOP;
        const uf8 read_count = sfifo8_get(&this->combuf, &com, 1);
		if(1 == read_count){
            if(SND_COM_COUNT > com){
                snd_coms_handlers[com](this);
            }
        }

        if(SND_STAT_COUNT > this->stat){
            snd_stat_handlers[this->stat](this);
        }
	}
}

/**
 * Initialize sound engine
 * @param sound_entry - entry for sound engine
 * @param backend - backend (driver) for sound engine
 * @return SUCCESS if succes,  otherwise e_sound_engine_status_error
 */
void snd_engine_init(THIS,
		     		 const snd_conf_t * restrict const conf)
{
    memclr(this, sizeof(snd_entry_t));
	
	this->conf = conf;
	this->input = conf->input;

	this->stat = SND_STAT_IDLE;
	this->codec = SND_CODEC_NONE;
	this->volume = SND_DEFAULT_VOLUME;
	
	sfifo8_init(&this->combuf, this->combuf_data, SOUND_COMMAND_BUFFER_SIZE);

	minit(this->mutex);
	cinit(this->cond);
	/* create thread */
	TaskCreate(this, snd_routine, SND_STACK_SIZE, SND_PRIORITY);
}

/* interface */

/**
 * start playing melody
 * @param snd		- [inout] sound device
 * @param sound		- [in] sound to play
 * 
 * @return 		- true if success,
 * 			  false else
 */
b8 snd_sound_play(THIS,
		  		  const sound_t * restrict const sound,
				  const u8 play_cnt)
{
	const u8 block[6] = {
		[0] = SND_PLAY,
		/* little endian */
		[1] = (u8)(((u32)sound >> 24)&0xff),
		[2] = (u8)(((u32)sound >> 16)&0xff),
		[3] = (u8)(((u32)sound >> 8)&0xff),
		[4] = (u8)(((u32)sound >> 0)&0xff),
		[5] = play_cnt,
	};

	return snd_com(this, block, 6);
}

/**
 * stop playing melody
 * @param snd		- sound device
 * @return 		- true if stop req generated
 */
b8 snd_sound_stop(THIS)
{
	static const u8 stop_com = SND_STOP;
	return snd_com(this, &stop_com, 1);	
}

/**
 * set new volume
 * @param snd		- sound device
 * @param new_volume	- new volume
 * @return 		- true if success, false else
 */
b8 snd_volume_set(THIS,
				  const u8 new_volume)
{
	const u8 block[2] = {
		[0] = SND_VOLUME,
		[1] = new_volume,
	};

	return snd_com(this, block, 2);
}

/**
 * zero all routine work area, and clear pbuf
 * 
 * @param this
 */
void snd_clear(THIS){
    memclr(&this->rwa, sizeof(this->rwa));
	memclr(&this->gen, sizeof(this->gen));
}