#ifndef CMT2300A_MODULE
#error "must be include just into cmt2300a.c"
#endif
#include "cmt2300a.h"
#include "cmt2300a_registers.h"

#define THIS cmt2300a_t * const this

static
b8 cmt2300a_transmit(THIS,
                     const u8 cnt,
                     const slice_t* slices,
                     const u8 repeat_cnt);

static
s32 cmt2300a_receive(THIS,
					 slice_t * slices,
					 const u32 size,
					 const u32 msec);

/**
 * turn device to sleep mode
 * @param be		- backend
 * @return		- true if success
 */
static
b8 cmt2300a_sleep(THIS);
/**
 * wake device from sleep
 * @param be		- backend
 * @return		- true if success
 */
static
b8 cmt2300a_wake(THIS);

/**
 * turn device off
 * @param be		- backend
 * @return		- true if success
 */
static
b8 cmt2300a_off(THIS);

/**
 * turn device on
 * @param be		- backend
 * @return		- true if success
 */
static
b8 cmt2300a_on(THIS);

/**
 * set central modulation freq
 * @param be		- backend
 * @param freq		- freq identifier
 * @return 		- true if success
 */
//static
//b8 cmt2300a_freq(THIS, const u32 freq);

static const
ll_be_ft_t cmt2300a_ft = {
    .transmit = (ll_be_transmit_t)cmt2300a_transmit,
	.receive = (ll_be_receive_t)cmt2300a_receive,
	.sleep = (ll_be_sleep_t)cmt2300a_sleep,
	.wake = (ll_be_wake_t)cmt2300a_wake,
	//.freq = (ll_be_freq_t)cmt2300a_freq,
	.on = (ll_be_on_t)cmt2300a_on,
	.off = (ll_be_off_t)cmt2300a_off,
};


static const u8 cmt2300a_goto_eeprom[] = {CUS_MODE_CTL, CMT2300A_GO_EEPROM};
static const u8 cmt2300a_goto_stby[] = {CUS_MODE_CTL, CMT2300A_GO_STBY};
static const u8 cmt2300a_goto_rfs[] = {CUS_MODE_CTL, CMT2300A_GO_RFS};
static const u8 cmt2300a_goto_rx[] = {CUS_MODE_CTL, CMT2300A_GO_RX};
static const u8 cmt2300a_goto_sleep[] = {CUS_MODE_CTL, CMT2300A_GO_SLEEP};
static const u8 cmt2300a_goto_tfs[] = {CUS_MODE_CTL, CMT2300A_GO_TFS};
static const u8 cmt2300a_goto_tx[] = {CUS_MODE_CTL, CMT2300A_GO_TX};
static const u8 cmt2300a_goto_switch[] = {CUS_MODE_CTL, CMT2300A_GO_SWITCH};

#define CMT2300A_GPIO_CONF(__TX__, __GNUM__)    ((__TX__ << 7) | ((__GNUM__ - 1) << 5))

static const u8 cmt2300a_config[] = {
    CUS_CMT1, 0x00,  // 00
    CUS_CMT2, 0x66,  // 01
    CUS_CMT3, 0xEC,  // 02
    CUS_CMT4, 0x1D,  // 03
    CUS_CMT5, 0x30,  // 04  [0xF0 - GFSK. 0x30 OOK]
    CUS_CMT6, 0x80,  // 05
    CUS_CMT7, 0x14,  // 06
    CUS_CMT8, 0x08,  // 07
    CUS_CMT9, 0x91,  // 08
    CUS_CMT10, 0x02, // 09
    CUS_CMT11, 0x02, // 0A
    CUS_RSSI, 0xD0,  // 0B
    CUS_SYS1, 0xAE,  // 0C
    CUS_SYS2, 0xE0,  // 0D
    CUS_SYS3, 0x35,  // 0E
    CUS_SYS6, 0xF4,  // 11
    CUS_SYS7, 0x10,  // 12
    CUS_SYS8, 0xE2,  // 13
    CUS_SYS9, 0x42,  // 14
    CUS_SYS10, 0x20, // 15
    CUS_SYS11, 0x10, // 16 0x00 19.01.2022
    CUS_SYS12, 0x81, // 17
    /* 433.920 */
    CUS_RF1, 0x42,                        // 18
    CUS_RF2, 0x71,                        // 19
    CUS_RF3, 0xCE,                        // 1A
    CUS_RF4, 0x1C,                        // 1B
    CUS_RF5, 0x42,                        // 1C
    CUS_RF6, 0x5B,                        // 1D
    CUS_RF7, 0x1C,                        // 1E
    CUS_RF8, 0x1C,                        // 1F
    CUS_RF9, 0x2A,                        // 20
    CUS_RF10, 0x83,                       // 21
    CUS_RF11, CMT2300A_OOK_BANDWIDTH_200, // 22
    CUS_RF12, 0x33,                       // 23 0xA2
    CUS_CDR1, 0x29,                       // 2B CDR TYPE: 2B 0x29 14.01.2022 0x2D IS COUNTING ----
    CUS_CDR2, 0xC0,                       // 2C [0x80]
    CUS_CDR3, 0xA5,                       // 2D
    CUS_CDR4, 0x02,                       // 2E
    CUS_AGC1, 0x6B,                       // 2F 53 ------------------------------
    CUS_AGC2, 0x17,                       // 30
    CUS_AGC3, 0x00,                       // 31
    CUS_AGC4, 0x50,                       // 32
    /* if FSK modulation 0x33-0x37 ignored */
    CUS_OOK1, 0x15,   // 33
    CUS_OOK2, 0x00,   // 34
    CUS_OOK3, 0x01,   // 35
    CUS_OOK4, 0x05,   // 36
    CUS_OOK5, 0x05,   // 37
    CUS_TX1, 0x45,    // 55 0x73 19.01.2022    0x41(bit2 = 0) - packet 0x45 - (bit2 = 1)pedal 04.oct.2022
    CUS_TX2, 0x9A,    // 56 FB lsb deveation
    CUS_TX3, 0x0C,    // 57 0C msb deveation
    CUS_TX6, 0x50,    // 5A
    CUS_TX8, 0x8A,    // 5C // __>  tx power 20dbm
    CUS_TX9, 0x18,    // 5D // -->  CMT2300A_TRX_20
    CUS_TX10, 0x3F,   // 5E
    CUS_IO_SEL, 0x18, // 65 // Setup GPIO 1,2,3 DIN DOUT = 0x18
};

static const u8 cmt2300a_back_config[] = {CUS_PKT17, 0xAA};
