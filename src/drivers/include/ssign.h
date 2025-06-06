#ifndef SSIGN_H
#define SSIGN_H
#include "ia_arch.h"
#include "dport.h"
#include "sfifo8.h"
#include "slice.h"

#define SSIGN_STACK_SIZE	(configMINIMAL_STACK_SIZE)
#define SSIGN_PRIORITY		(12)
#define SSIGN_COM_BUFF      (8)

typedef enum {
    SMODE_FLASH_P = 0,
    SMODE_MOTOR_P,

    SMODE_NONE = 0,
    SMODE_FLASH = (1 << SMODE_FLASH_P),
    SMODE_MOTOR = (1 << SMODE_MOTOR_P),
    SMODE_FULL = (1 << SMODE_MOTOR_P) | (1 << SMODE_FLASH_P),
} ssign_mode_t;

typedef enum {
    SCOM_NONE = 0,
    SCOM_IND,
} ssign_com_t;

typedef enum __attribute__((packed)) {
	ALD_FIRST = 0,
	ALD_TEST = 0,
	ALD_BABY,
	ALD_DBELL1,
	ALD_DBELL2,
	ALD_ICOM,
	ALD_PLLINE,
	ALD_PCELL,
	ALD_SMOKE,
	ALD_DWOPEN,
	ALD_WATER,
	ALD_ASSIST,
	ALD_GAS,
	ALD_CALARM,
	ALD_COMMON_COUNT,
	ALD_PWM_ON,
	ALD_PWM_OFF,
	ALD_PWM_PULS,
	ALD_PWM_CONNECT,
	ALD_COUNT,
} alarm_description_t;

typedef struct {
    u8 len;
    slice_t slice[0];
} ssign_pattern_t;

typedef struct {
    DPORT l_port;
    DPORT m_port;
    u8 l_pin;
    u8 m_pin;
} ssign_conf_t;

typedef struct {
    DPORT l_port;
    DPORT m_port;
    u16 l_mask;
    u16 m_mask;

    struct {
        ssign_com_t curr_com;
        ssign_mode_t curr_mode;

        u8 pos;
        const ssign_pattern_t * pattern;
    } proc;

    sfifo8_t com;
    u8 combuff[SSIGN_COM_BUFF];

    tdc_t cond;
    tdm_t mutex;

    TASK_T(SSIGN_STACK_SIZE);
} ssign_t;

extern
void ssign_init(ssign_t * const this, const ssign_conf_t * const conf);

extern
void ssign_stop(ssign_t * const this);

extern
void ssign_set(ssign_t * const this, const ssign_mode_t mode, const alarm_description_t ind);

#endif//SSIGN_H

#if defined(DEVICE_TREE_H)
extern ssign_t ssign;
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(SSIGN_DEVICE_TREE)
#define SSIGN_DEVICE_TREE

ssign_t ssign;

static const 
ssign_conf_t ssign_conf = {
    .l_port = SSIGN_L_PORT,
    .m_port = SSIGN_M_PORT,
    .l_pin = SSIGN_L_PIN,
    .m_pin = SSIGN_M_PIN,
};

#define SSIGN_INIT()    ssign_init(&ssign, &ssign_conf)

#endif
