/* simple driver that provide iface for up to 8 buttons,
 * based on gpio and located on same gpio PORT
 */
#define KEYPAD_MODULE
#include "keypad_i.h"

/***
 * @brief get maxtrix keypad output data
 * @return 			- every bit is pressed state of selected button
 */
__attribute__((always_inline))
static inline
u32 kp_get(MOD)
{
	u32 all_pressed = 0;
	for(uf8 sel = 0; BUTTON_COUNT > sel; ++sel){
		const b8 btn_pressed = (0 == DPORT_GET(this->dport[sel], this->mask[sel]));
		all_pressed |= ((u32)btn_pressed << sel);
	}

	return all_pressed;
}

/***
 * @brief emit press event
 * @param btnnum		- [in] button number
 */ 
__attribute__((always_inline))
static inline
void kp_event_prs(MOD,
				    const u8 btnnum)
{
	/* direct event for every button */
	const inevent_events_t btnevent = (BTN_EOFFSET + 1) + (3 * btnnum) + BTN_P_OFFSET;
	inevent_event(this->input,
					btnevent);
}

/***
 * @brief emit release event
 * @param btnnum		- [in] button number
 */ 
__attribute__((always_inline))
static inline
void kp_event_rel(MOD,
					const u8 btnnum)
{
	/* direct event for every button */
	const inevent_events_t btnevent = (BTN_EOFFSET + 1) + (3 * btnnum) + BTN_R_OFFSET;
	inevent_event(this->input,
					btnevent);
}

/***
 * @brief emit release event
 * @param btnnum		- [in] button number
 * @param hold_time		- [in] hold time
 */ 
__attribute__((always_inline))
static inline
void kp_event_hold(MOD,
				   const u8 btnnum,
				   const u8 hold_time)
{
	/* direct event for every button */
	const inevent_events_t btnevent = (BTN_EOFFSET + 1) + (3 * btnnum) + BTN_H_OFFSET;
	inevent_event_arg1(this->input,
						btnevent,
						hold_time);
}

/**
 * @brief main keypad routine
 */
static
void kp_routine(MOD)
{
	/* init part */
	mlock(this->mutex);
	exti_group_t ext_mask = 0;
	for(uf8 sel = 0; BUTTON_COUNT > sel; ++sel){
		DPORT port = this->dport[sel];
		const u8 pin = this->pin[sel];
		this->mask[sel] = (1 << pin);
		DPORT_DIN_INIT_WPU(port, this->mask[sel]);
		ext_mask |= extint_configure(port, pin, EXTI_Trigger_Falling);
	}


	/* button handling */
	for(;;){
		const u32 btn_input = kp_get(this);

		uf8 mask = (1 << 0);
		b8 need_attention = false;
		uf8 new_state = 0;
		
		for(uf8 sel = 0; BUTTON_COUNT > sel; ++sel, mask <<= 1){
			const b8 pressed = (0 < (btn_input & mask));
			const b8 was_pressed = (0 < (mask & this->press));
			/* handle buttons */
			if(unlikely(true == pressed)){
				if(true == was_pressed){
					/* button holded */
					this->hold |= mask;
					const uf8 hcycles = this->hcycles[sel];
					this->hcycles[sel] = 0xff > hcycles ? hcycles + 1: 0xff;
					kp_event_hold(this, sel, hcycles);
				} else {
					/* button pressed */
					this->press |= mask;
					kp_event_prs(this, sel);
				}
				need_attention = true;
				new_state |= mask;
			}else{
				if(true == was_pressed){
					/* button release */
					kp_event_rel(this, sel);
				}
				this->press &= ~(mask);
				this->hold &= ~(mask);
				this->hcycles[sel] = 0;
			}
		}
		this->state = new_state;

		mulock(this->mutex);
		if(unlikely(false == need_attention)){
			/* wait for press event */
			extint_wait(ext_mask, portMAX_DELAY);
		} else {
			msleep(PROC_TIME_MS);
		}
		mlock(this->mutex);
	}
}

		/*** interface ***/

/**
 * @brief return state of selected button
 * @param this		- [in] keypad instance
 * @param num		- [in] button number
 * 
 * @return 			- button state
 */
kpse_t kp_state(MOD,
				const u8 num)
{
	mlock(this->mutex);
	const u8 press = this->press;	
	const u8 hold = this->hold;
	mulock(this->mutex);

	const u8 spress = press >> num;
	const u8 shold = hold >> num;
	const kpse_t state = (kpse_t)(spress + (shold << 1));
		
	return state;
}

/**
 * @brief init keypad module
 * @param conf		- [in] keypad config
 */
b8 kp_ctor(MOD,
	       const keypad_conf_t * restrict conf)
{
	memclr(this, sizeof(keypad_t));
	this->input = conf->input;
	
	for(uf8 sel = 0; BUTTON_COUNT > sel; ++sel){
		this->pin[sel] = conf->pin[sel];
		this->dport[sel] = conf->dport[sel];
	}

	this->press = 0;
	this->hold = 0;
	this->state = 0;
	
	minit(this->mutex);

	TaskCreate(this, kp_routine, KEYPAD_STACK_SIZE, KEYPAD_PRIORITY);
	return true;
}
