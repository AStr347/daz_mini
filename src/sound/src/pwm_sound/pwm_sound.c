/**
 * simple i2s driver, that use spi channel 2 in i2s mode
 * driver also use DMA channel 0
 * 
 * todo: add the brg translation list
 * for easy brg calculation
 * now brg define in acrh/boards
 */

#define I2S_MODULE		0x000000011
#include "pwm_sound_i.h"

static
b8 pwm_sound_on(THIS){
	if(SND_BE_OFF == this->mode){
		this->mode = SND_BE_READY;
	}
	return (SND_BE_READY == this->mode);
}

static
void pwm_sound_off(THIS){
	if(SND_BE_PLAYING == this->mode){
		pwm_stop(this->pwm);
	}
	this->mode = SND_BE_OFF;
}

static
void pwm_sound_start(THIS, const samplebuffer_t * const buff){
	if(SND_BE_READY == this->mode){
		pwm_start_circ_tx(this->pwm, this->pwm_ch, (u16*)buff->data, SAMPLEBUFFER_SIZE);
		this->mode = SND_BE_PLAYING;
	}
}

static
void pwm_sound_stop(THIS){
	if(SND_BE_PLAYING == this->mode){
		pwm_stop(this->pwm);
	}
}

static
b8 pwm_sound_wait(THIS){
	b8 result = false;
	if(SND_BE_PLAYING == this->mode){
		result = pwm_wait(this->pwm);
	}
	return result;
}

snd_be_t * pwm_sound_init(THIS, const pwms_config_t * const conf)
{
	memclr(this, sizeof(pwms_entry_t));
	snd_be_t * be = &this->be;
	be->ft = &pwm_sound_ft;
	be->this = this;

	this->pwm = conf->pwm;
	this->pwm_ch = conf->pwm_ch - 1;
	this->mode = SND_BE_OFF;

	return be;
}
