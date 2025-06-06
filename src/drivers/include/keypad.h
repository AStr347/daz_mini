/* simple driver that provide iface for up to 8 buttons,
 * based on gpio and located on same gpio PORT
 */
#ifndef KEYPAD_H
#define KEYPAD_H
#include "ia_arch.h"
#include "extint.h"
#include "string.h"
#include "inevent.h"
#include "dport.h"

#define KEYPAD_STACK_SIZE	(configMINIMAL_STACK_SIZE)

#ifndef BUTTON_COUNT
#define BUTTON_COUNT (0)
#endif

/* keypad button states */
typedef enum __packed {
	kpse_no		= 0,
	kpse_prs	= 1, /* button press */	
	kpse_rel	= 2, /* button release */
	kpse_hld	= 3, /* button hold */
} kpse_t;

/**
 * keypad simple instance
 */
typedef struct keypad {
	inevent_t* restrict input;		/* inevent subsystem to use */
	DPORT dport[BUTTON_COUNT];		/* button port to use */
	u8 pin[BUTTON_COUNT];			/* input, wpu pin number, that we read when need to get data */
	u16 mask[BUTTON_COUNT];			/* row: input, wpu pin mask, that we read when need to get data */
	u8 hcycles[BUTTON_COUNT];		/* hold cycles(passed in every hold event) */

	tdm_t mutex;					/* mutex to safe access */

	volatile u8 state;
	volatile u8 press;				/* what buttons is pressed now */
	volatile u8 hold;				/* what buttons is holded now */

	TASK_T(KEYPAD_STACK_SIZE);
} keypad_t;

/** 
 * keypad config
 * @mem backend 	- module's backend, used for raw fill
 */
typedef struct keypad_conf {
	inevent_t* restrict input;
	DPORT dport[BUTTON_COUNT];		/* port to use */
	u8 pin[BUTTON_COUNT];			/* input, wpu pin number, that we read when need to get data */
}keypad_conf_t;


/*** interface functions ***/

/**
 * @brief init keypad module
 * @param this		- [out] keypad instance
 * @param conf		- [in] keypad config
 * 
 * @return			- true, if success false if module
 * 					  is not initiated
 */
extern
b8 kp_ctor(keypad_t * restrict this,
	       const keypad_conf_t * restrict conf);

/**
 * @brief return state of selected button
 * @param this		- [in] keypad instance
 * @param num		- [in] button number
 * 
 * @return 			- button state
 */
extern
kpse_t kp_state(keypad_t * restrict const this,
		 	    const u8 num);


#define KEYPAD_BTN_IS_PRESSED(__inst__, __btn__)	(kpse_hld == kp_state((__inst__), (__btn__)))

#endif

#if defined(DEVICE_TREE_H)
extern keypad_t keypad0;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(KEYPAD_DEVICE_TREE)
#define KEYPAD_DEVICE_TREE

/* serial keypad instance,
 * only one serial keypad can present in system
 */
keypad_t keypad0;

static
const keypad_conf_t keypad0_conf = {
	.dport[0] = KEYPAD_DPORT0,
    .pin[0] = KEYPAD_PIN0,

#if (1 < BUTTON_COUNT)
	.dport[1] = KEYPAD_DPORT1,
    .pin[1] = KEYPAD_PIN1,
#endif

#if (2 < BUTTON_COUNT)
	.dport[2] = KEYPAD_DPORT2,
    .pin[2] = KEYPAD_PIN2,
#endif

#if (3 < BUTTON_COUNT)
	.dport[3] = KEYPAD_DPORT3,
    .pin[3] = KEYPAD_PIN3,
#endif

#if (4 < BUTTON_COUNT)
	.dport[4] = KEYPAD_DPORT4,
    .pin[4] = KEYPAD_PIN4,
#endif

#if (5 < BUTTON_COUNT)
	.dport[5] = KEYPAD_DPORT5,
    .pin[5] = KEYPAD_PIN5,
#endif

#if (6 < BUTTON_COUNT)
	.dport[6] = KEYPAD_DPORT6,
    .pin[6] = KEYPAD_PIN6,
#endif

#if (7 < BUTTON_COUNT)
	.dport[7] = KEYPAD_DPORT7,
    .pin[7] = KEYPAD_PIN7,
#endif
    .input = &INE_DEFAULT,
};
#define KEYPAD_INIT()		kp_ctor(&keypad0, &keypad0_conf)
#endif

