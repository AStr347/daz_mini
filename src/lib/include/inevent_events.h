#ifndef INEVENT_EVENTS
#define INEVENT_EVENTS
#include "board.h"

#if defined(RTC_DRV)
#define TIME_EVENTS()\
	EVENT_TIME_DSEC,\
	EVENT_TIME_HSEC,\
	EVENT_TIME_SEC,\
	EVENT_TIME_MIN,\
    EVENT_TIME_HOUR,\
    EVENT_TIME_DAY,

#define ON_DSEC()		static void __dsec_handler(void)
#define ON_HSEC()		static void __hsec_handler(void)
#define ON_SEC()		static void __sec_handler(void)
#define ON_MIN()		static void __min_handler(void)
#define ON_HOUR()		static void __hour_handler(void)
#define ON_DAY()		static void __day_handler(void)

#define DSEC_EVENT	{.event = EVENT_TIME_DSEC, .handler = (inevent_slot_handler)__dsec_handler}
#define HSEC_EVENT	{.event = EVENT_TIME_HSEC, .handler = (inevent_slot_handler)__hsec_handler}
#define SEC_EVENT	{.event = EVENT_TIME_SEC, .handler = (inevent_slot_handler)__sec_handler}
#define MIN_EVENT	{.event = EVENT_TIME_MIN, .handler = (inevent_slot_handler)__min_handler}
#define HOUR_EVENT	{.event = EVENT_TIME_HOUR, .handler = (inevent_slot_handler)__hour_handler}
#define DAY_EVENT	{.event = EVENT_TIME_DAY, .handler = (inevent_slot_handler)__day_handler}

#else
#define TIME_EVENTS()
#endif
	
#if defined(BUTTONS)

enum {
    BTN_P_OFFSET = 0,
    BTN_R_OFFSET = 1,
    BTN_H_OFFSET = 2,
};

#ifdef ENTER_BTN
#define ENTER_EVENTS()\
	EVENT_ENTER_P,\
	EVENT_ENTER_R,\
	EVENT_ENTER_H,

#define ON_ENTER_P()		static void __enter_p_handler(void)
#define ON_ENTER_R()		static void __enter_r_handler(void)
#define ON_ENTER_H(time)	static void __enter_h_handler(time)
#define ENTER_P_EVENT	{.event = EVENT_ENTER_P, .handler = (inevent_slot_handler)__enter_p_handler}
#define ENTER_R_EVENT	{.event = EVENT_ENTER_R, .handler = (inevent_slot_handler)__enter_r_handler}
#define ENTER_H_EVENT	{.event = EVENT_ENTER_H, .handler = (inevent_slot_handler)__enter_h_handler}

#else
#define ENTER_EVENTS()
#endif

#ifdef CANSEL_BTN
#define CANSEL_EVENTS()\
	EVENT_CANSEL_P,\
	EVENT_CANSEL_R,\
	EVENT_CANSEL_H,

#define ON_CANSEL_P()		static void __cansel_p_handler(void)
#define ON_CANSEL_R()		static void __cansel_r_handler(void)
#define ON_CANSEL_H(time)	static void __cansel_h_handler(time)
#define CANSEL_P_EVENT	{.event = EVENT_CANSEL_P, .handler = (inevent_slot_handler)__cansel_p_handler}
#define CANSEL_R_EVENT	{.event = EVENT_CANSEL_R, .handler = (inevent_slot_handler)__cansel_r_handler}
#define CANSEL_H_EVENT	{.event = EVENT_CANSEL_H, .handler = (inevent_slot_handler)__cansel_h_handler}

#else
#define CANSEL_EVENTS()
#endif

#ifdef ESCAPE_BTN
#define ESCAPE_EVENTS()\
	EVENT_ESCAPE_P,\
	EVENT_ESCAPE_R,\
	EVENT_ESCAPE_H,

#define ON_ESCAPE_P()		static void __escape_p_handler(void)
#define ON_ESCAPE_R()		static void __escape_r_handler(void)
#define ON_ESCAPE_H(time)	static void __escape_h_handler(time)
#define ESCAPE_P_EVENT	{.event = EVENT_ESCAPE_P, .handler = (inevent_slot_handler)__escape_p_handler}
#define ESCAPE_R_EVENT	{.event = EVENT_ESCAPE_R, .handler = (inevent_slot_handler)__escape_r_handler}
#define ESCAPE_H_EVENT	{.event = EVENT_ESCAPE_H, .handler = (inevent_slot_handler)__escape_h_handler}

#else
#define ESCAPE_EVENTS()
#endif

#ifdef LEFT_BTN
#define LEFT_EVENTS()\
	EVENT_LEFT_P,\
	EVENT_LEFT_R,\
	EVENT_LEFT_H,

#define ON_LEFT_P()		static void __left_p_handler(void)
#define ON_LEFT_R()		static void __left_r_handler(void)
#define ON_LEFT_H(time)	static void __left_h_handler(time)
#define LEFT_P_EVENT	{.event = EVENT_LEFT_P, .handler = (inevent_slot_handler)__left_p_handler}
#define LEFT_R_EVENT	{.event = EVENT_LEFT_R, .handler = (inevent_slot_handler)__left_r_handler}
#define LEFT_H_EVENT	{.event = EVENT_LEFT_H, .handler = (inevent_slot_handler)__left_h_handler}

#else
#define LEFT_EVENTS()
#endif

#ifdef RIGHT_BTN
#define RIGHT_EVENTS()\
	EVENT_RIGHT_P,\
	EVENT_RIGHT_R,\
	EVENT_RIGHT_H,

#define ON_RIGHT_P()		static void __right_p_handler(void)
#define ON_RIGHT_R()		static void __right_r_handler(void)
#define ON_RIGHT_H(time)	static void __right_h_handler(time)
#define RIGHT_P_EVENT	{.event = EVENT_RIGHT_P, .handler = (inevent_slot_handler)__right_p_handler}
#define RIGHT_R_EVENT	{.event = EVENT_RIGHT_R, .handler = (inevent_slot_handler)__right_r_handler}
#define RIGHT_H_EVENT	{.event = EVENT_RIGHT_H, .handler = (inevent_slot_handler)__right_h_handler}

#else
#define RIGHT_EVENTS()
#endif

#ifdef UP_BTN
#define UP_EVENTS()\
	EVENT_UP_P,\
	EVENT_UP_R,\
	EVENT_UP_H,

#define ON_UP_P()		static void __up_p_handler(void)
#define ON_UP_R()		static void __up_r_handler(void)
#define ON_UP_H(time)	static void __up_h_handler(time)
#define UP_P_EVENT	{.event = EVENT_UP_P, .handler = (inevent_slot_handler)__up_p_handler}
#define UP_R_EVENT	{.event = EVENT_UP_R, .handler = (inevent_slot_handler)__up_r_handler}
#define UP_H_EVENT	{.event = EVENT_UP_H, .handler = (inevent_slot_handler)__up_h_handler}

#else
#define UP_EVENTS()
#endif

#ifdef DOWN_BTN
#define DOWN_EVENTS()\
	EVENT_DOWN_P,\
	EVENT_DOWN_R,\
	EVENT_DOWN_H,

#define ON_DOWN_P()		static void __down_p_handler(void)
#define ON_DOWN_R()		static void __down_r_handler(void)
#define ON_DOWN_H(time)	static void __down_h_handler(time)
#define DOWN_P_EVENT	{.event = EVENT_DOWN_P, .handler = (inevent_slot_handler)__down_p_handler}
#define DOWN_R_EVENT	{.event = EVENT_DOWN_R, .handler = (inevent_slot_handler)__down_r_handler}
#define DOWN_H_EVENT	{.event = EVENT_DOWN_H, .handler = (inevent_slot_handler)__down_h_handler}

#else
#define DOWN_EVENTS()
#endif

#ifdef MIC_BTN
#define MIC_EVENTS()\
	EVENT_MIC_P,\
	EVENT_MIC_R,\
	EVENT_MIC_H,

#define ON_MIC_P()		static void __mic_p_handler(void)
#define ON_MIC_R()		static void __mic_r_handler(void)
#define ON_MIC_H(time)	static void __mic_h_handler(time)
#define MIC_P_EVENT	{.event = EVENT_MIC_P, .handler = (inevent_slot_handler)__mic_p_handler}
#define MIC_R_EVENT	{.event = EVENT_MIC_R, .handler = (inevent_slot_handler)__mic_r_handler}
#define MIC_H_EVENT	{.event = EVENT_MIC_H, .handler = (inevent_slot_handler)__mic_h_handler}

#else
#define MIC_EVENTS()
#endif
	
#define BTN_EVENTS()\
	BTN_EOFFSET,\
	ENTER_EVENTS()\
	CANSEL_EVENTS()\
    ESCAPE_EVENTS()\
	LEFT_EVENTS()\
	RIGHT_EVENTS()\
	UP_EVENTS()\
	DOWN_EVENTS()\
    MIC_EVENTS()
#else
#define BTN_EVENTS()
#endif

#if defined(UPOWER)
#define UPOWER_EVENTS()\
	EVENT_POWER_U,

#define ON_UPOWER(stat)         static void __upower_handler(stat)
#define UPOWER_EVENT		{.event = EVENT_POWER_U, .handler = (inevent_slot_handler)__upower_handler}

#else
#define UPOWER_EVENTS()
#endif

#ifdef NET
#define NET_EVENTS()\
	EVENT_NET_INFO,\
    EVENT_NET_TEST,\
	EVENT_NET_CANCEL,

#define ON_NET()            static void __net_info_handler(void)
#define NET_INFO_EVENT		{.event = EVENT_NET_INFO, .handler = (inevent_slot_handler)__net_info_handler}

#define ON_NET_CANCEL()		static void __net_cancel_handler(void)
#define NET_CANCEL_EVENT	{.event = EVENT_NET_CANCEL, .handler = (inevent_slot_handler)__net_cancel_handler}

#define ON_NET_TEST()       static void __net_test_handler(void)
#define NET_TEST_EVENT		{.event = EVENT_NET_TEST, .handler = (inevent_slot_handler)__net_test_handler}

#else 
#define NET_EVENTS()
#endif

#ifdef NET_LEGACY
#define NETL_EVENTS()\
	EVENT_NETL_INFO,\
    EVENT_NETL_PING,\
    EVENT_NETL_REPEAT,

#define ON_NETL_INFO()		static void __netl_info_handler(void)
#define NETL_INFO_EVENT		{.event = EVENT_NETL_INFO, .handler = (inevent_slot_handler)__netl_info_handler}

#define ON_NETL_PING(extra) static void __netl_ping_handler(extra)
#define NETL_PING_EVENT		{.event = EVENT_NETL_PING, .handler = (inevent_slot_handler)__netl_ping_handler}

#define ON_NETL_REPEAT()    static void __netl_repeat_handler(void)
#define NETL_REPEAT_EVENT	{.event = EVENT_NETL_REPEAT, .handler = (inevent_slot_handler)__netl_repeat_handler}

#else
#define NETL_EVENTS()
#endif

#ifdef SOUND
#define SOUND_EVENTS()\
	EVENT_SOUND_PLAY_END,\
	EVENT_SOUND_REC_END,

#define ON_SOUND_END()		static void __sound_end_handler(void)
#define SOUND_END_EVENT		{.event = EVENT_SOUND_PLAY_END, .handler = (inevent_slot_handler)__sound_end_handler}

#define ON_REC_END()		static void __rec_end_handler(void)
#define REC_END_EVENT		{.event = EVENT_SOUND_REC_END, .handler = (inevent_slot_handler)__rec_end_handler}

#else
#define SOUND_EVENTS()
#endif

#ifdef USB
#define USB_EVENTS()\
	EVENT_USB_PLUG,

#define ON_USB_PLUG()		static void __usb_plug_handler(void)
#define USB_PLUG_EVENT		{.event = EVENT_USB_PLUG, .handler = (inevent_slot_handler)__usb_plug_handler}

#else
#define USB_EVENTS()
#endif

#ifdef MOTION_SENSOR_USER
#define MOTION_EVENTS()\
	EVENT_MOTION_SENSOR,

#define ON_MOTION()		static void __motion_handler(void)
#define MOTION_EVENT		{.event = EVENT_MOTION_SENSOR, .handler = (inevent_slot_handler)__motion_handler}

#else
#define MOTION_EVENTS()
#endif

#ifdef BROADCAST_LINE
#define LINE_EVENTS()\
	EVENT_LINE_BCAST,\
	EVENT_LINE_OFF,

#define ON_LINE_BCAST()		static void __line_bcast_handler(void)
#define LINE_BCAST_EVENT	{.event = EVENT_LINE_BCAST, .handler = (inevent_slot_handler)__line_bcast_handler}

#define ON_LINE_OFF()		static void __line_off_handler(void)
#define LINE_OFF_EVENT		{.event = EVENT_LINE_OFF, .handler = (inevent_slot_handler)__line_off_handler}

#else
#define LINE_EVENTS()
#endif

#if defined(BABYCRY) || defined(MEMSCRY)
#define BABYCRY_EVENTS()\
    EVENT_BABYCRY,

#define ON_BABYCRY()    static void __baby_cry_handler(void)
#define BABYCRY_EVENT	{.event = EVENT_BABYCRY, .handler = (inevent_slot_handler)__baby_cry_handler}
#else
#define BABYCRY_EVENTS()
#endif

/* For a while event ble CTL will be used
 * to add/remove buttons from PDA
 */
#ifdef BLE_DRV
/* This macro will be used to control actions,
 * that done when command from PDA received
 */

/* ADD button with specified ID */
#define BLECTL_CONN_ADD        (0) 
/* DEL button with specified ID */
#define BLECTL_CONN_DEL        (1)

#define BLE_DRV_EVENTS()\
    EVENT_BLE_INFO,\
    EVENT_BLE_CONN,\
    EVENT_BLE_CTL,\
    EVENT_BLE_PAIRING,

#define ON_BLE_INFO()     static void __ble_info_handler(void)
#define BLE_INFO_EVENT    {.event = EVENT_BLE_INFO, .handler = (inevent_slot_handler)__ble_info_handler}
#define ON_BLE_CONN(__CONNECTED__)     static void __ble_conn_handler(__CONNECTED__)
#define BLE_CONN_EVENT    {.event = EVENT_BLE_CONN, .handler = (inevent_slot_handler)__ble_conn_handler}
#define ON_BLE_CTL(__CONTROL__)     static void __ble_ctl_handler(__CONTROL__)
#define BLE_CTL_EVENT    {.event = EVENT_BLE_CTL, .handler = (inevent_slot_handler)__ble_ctl_handler}
#define ON_BLE_PAIRING(__COM__)     static void __ble_pairing_handler(__COM__)
#define BLE_PAIRING_EVENT    {.event = EVENT_BLE_PAIRING, .handler = (inevent_slot_handler)__ble_pairing_handler}

#else
#define BLE_DRV_EVENTS()
#endif

#ifdef CAPTOUCH
#define CAPTOUCH_EVENTS()\
    EVENT_CAPTOUCH,

#define ON_CAPTOUCH()     static void __captouch_handler(void)
#define CAPTOUCH_EVENT    {.event = EVENT_CAPTOUCH, .handler = (inevent_slot_handler)__captouch_handler}
#else
#define CAPTOUCH_EVENTS()
#endif

#ifdef DETECT_SENSOR
#define DETECT_SENSOR_EVENTS()\
    EVENT_DETECT_SENSOR,

#define ON_DETECT_SENSOR()     static void __detect_sensor_handler(void)
#define DETECT_SENSOR_EVENT    {.event = EVENT_DETECT_SENSOR, .handler = (inevent_slot_handler)__detect_sensor_handler}
#else
#define DETECT_SENSOR_EVENTS()
#endif

#ifdef EXTERNAL_SENSOR
#define EXTER_SENS_EVENTS()\
    EVENT_EXTER_SENS,

#define ON_EXTER_SENS(type)     static void __exter_handler(type)
#define EXTER_SENS_EVENT    {.event = EVENT_EXTER_SENS, .handler = (inevent_slot_handler)__exter_handler}
#else
#define EXTER_SENS_EVENTS()
#endif

#ifdef SIGNAL_SENSOR
#define SIGNAL_SENSOR_EVENTS()\
    EVENT_SIGNAL_SENSOR,

#define ON_SIGNAL_SENSOR()     static void __signal_sens_handler(void)
#define SIGNAL_SENSOR_EVENT    {.event = EVENT_SIGNAL_SENSOR, .handler = (inevent_slot_handler)__signal_sens_handler}
#else
#define SIGNAL_SENSOR_EVENTS()
#endif

#ifdef SOUND_SENSOR
#define SOUND_SENSOR_EVENTS()\
    EVENT_SOUND_SENSOR,

#define ON_SOUND_SENSOR()     static void __sound_sens_handler(void)
#define SOUND_SENSOR_EVENT    {.event = EVENT_SOUND_SENSOR, .handler = (inevent_slot_handler)__sound_sens_handler}
#else
#define SOUND_SENSOR_EVENTS()
#endif

#ifdef WATER_SENSOR
#define WATER_SENSOR_EVENTS()\
    EVENT_WATER_SENSOR,

#define ON_WATER_SENSOR()     static void __water_sens_handler(void)
#define WATER_SENSOR_EVENT    {.event = EVENT_WATER_SENSOR, .handler = (inevent_slot_handler)__water_sens_handler}
#else
#define WATER_SENSOR_EVENTS()
#endif

#ifdef CRADDLE
#define CRADDLE_EVENTS()\
    EVENT_CRADDLE,\
    EVENT_CRADDLE_INFO,

#define ON_CRADDLE(plug)     static void __craddle_handler(plug)
#define CRADDLE_EVENT    {.event = EVENT_CRADDLE, .handler = (inevent_slot_handler)__craddle_handler}

#define ON_CRADDLE_INFO(msg)     static void __craddle_info_handler(msg)
#define CRADDLE_INFO_EVENT    {.event = EVENT_CRADDLE_INFO, .handler = (inevent_slot_handler)__craddle_info_handler}
#else
#define CRADDLE_EVENTS()
#endif

#ifdef TOUCH
#define TOUCH_EVENTS()\
    EVENT_TOUCH,

#define ON_TOUCH(_xy)     static void __touch_handler(_xy)
#define TOUCH_EVENT    {.event = EVENT_TOUCH, .handler = (inevent_slot_handler)__touch_handler}
#else
#define TOUCH_EVENTS()
#endif

typedef enum __packed {
	EVENT_LIST = 0,
	EVENT_PROLOG,
	EVENT_EPILOG,
	TIME_EVENTS()
	BTN_EVENTS()
	UPOWER_EVENTS()
	NET_EVENTS()
    NETL_EVENTS()
	SOUND_EVENTS()
	USB_EVENTS()
	MOTION_EVENTS()
	LINE_EVENTS()
    BABYCRY_EVENTS()
    BLE_DRV_EVENTS()
    CAPTOUCH_EVENTS()
    DETECT_SENSOR_EVENTS()
    EXTER_SENS_EVENTS()
    SIGNAL_SENSOR_EVENTS()
    SOUND_SENSOR_EVENTS()
    WATER_SENSOR_EVENTS()
    CRADDLE_EVENTS()
    TOUCH_EVENTS()
    EVENT_CNT,
} inevent_events_t;


#define PROLOG()		static void __prolog_handler(void)
#define EPILOG()		static void __epilog_handler(void)
#define PROLOG_EVENT		{.event = EVENT_PROLOG, .handler = (inevent_slot_handler)__prolog_handler}
#define EPILOG_EVENT		{.event = EVENT_EPILOG, .handler = (inevent_slot_handler)__epilog_handler}


#endif//INEVENT_EVENTS
