/******************************************************************************
                              Copyright (c) 2017
                                  IA-Group
 ******************************************************************************/
#ifndef RB_CONFIG_H
#define	RB_CONFIG_H
#include "ia_arch.h"
#include "netlayer_generic.h"
#include "net_legacy.h"
#include "extsensor.h"
#include "board.h"
#include "macs.h"

/**
 * radio codes
 * @mem rc_wake_signal 		- signal that emit WAKE recceiver, when want to activate vibration pad
 */
typedef enum {
	RC_BABYCRY,
	RC_BABYCRY_HOPERF,
	RC_WAKE_SIGNAL,
	RC_BUTTON,
	SOS_BUTTON,
	RC_CELLPHONE_HOPERF,
	RC_TELEPHON,
	GSM_GATEWAY_COM3,
	RC_DOORPHONE,
	FIRE_ALARM,
	MOTION_SENSOR,
	RC_CALLHELP,
	RC_CALL,
	DOOR_WIND_OPENED,
	GASLEAK,
	WATERLEAK,
	CELLPHONE,
	RC_COUNT,	
	TEST_MSG,
	RC_MASK = (0b11111111UL),
} radio_code_t;

/* contain offset of init code in the pulsar3 encoding (50 addresses)*/

#define ADRESS_PREFFIX  2222L
#define RB_CODE_DEFAULT         (TRITGEN(ADRESS_PREFFIX,1010L)<<8)

/** Construct and fill preloaded paired list for digital signalize system
 * (for Nikita Arhipiov)
 * fill paired list with devices has same mac (one of 50 addresses) and set of
 * suffix which determinate device type 
 * 
 * if we use this in system helper case (for Alexandr Evstegneev) do nothing
 * @param net		- [inout] net to add connected
 * 
 * @return true if success
 */
extern
b8 pulsar3_fill_paired_list(void);

/**
 * generate id from given rafio code
 * @param type - radio_code_t
 * @return 
 */
extern
u64 pulsar3_id_from_msg_type(const radio_code_t type);

/** Translate extsensor event type to suffix for mac
 *  and construct mac addr to send for puls3
 * 
 * @param extsensor_data - extsensor event data
 * @return - modified mac addr with suffix according to extsensor event
 */
extern
u64 pulsar3_extsensor_to_mac_translator(const extsensors_t extsensor_data);

extern
const radio_code_t puls3_data_list[EXT_SENSORCOUNT];

/**
 * return id of mimic pulsar3
 */
extern
u64 pulsar3_devonlynet(const u64 id);


/**
 * fill only net devices if them mimic device exist
 */
extern
void pulsar3_devonlynet_fill(void);


#ifdef CRADDLE
/**
 * convert craddle signals to pulsar
 */
extern
const u8 craddle2pulsar[];
#endif

#endif	/* RB_CONFIG_H_ */
