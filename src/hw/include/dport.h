#ifndef DPORT_H
#define DPORT_H
#include "ia_arch.h"

/* gpio simply iface */
typedef GPIO_TypeDef* DPORT;

extern
void dport_init(DPORT port,
                const u16 pin,
                const GPIOSpeed_TypeDef speed,
                const GPIOMode_TypeDef mode);

#define DPORT_AN_INIT(__PORT__, __MASK__)\
    dport_init(__PORT__, __MASK__, GPIO_Speed_2MHz, GPIO_Mode_AIN)

#define DPORT_DIN_INIT(__PORT__, __MASK__)\
    dport_init(__PORT__, __MASK__, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING)

#define DPORT_DIN_INIT_WPD(__PORT__, __MASK__)\
    dport_init(__PORT__, __MASK__, GPIO_Speed_2MHz, GPIO_Mode_IPD)

#define DPORT_DIN_INIT_WPU(__PORT__, __MASK__)\
    dport_init(__PORT__, __MASK__, GPIO_Speed_2MHz, GPIO_Mode_IPU)

#define DPORT_DOUT_INIT_OD(__PORT__, __MASK__)\
    dport_init(__PORT__, __MASK__, GPIO_Speed_50MHz, GPIO_Mode_Out_OD)

#define DPORT_DOUT_INIT(__PORT__, __MASK__)\
    dport_init(__PORT__, __MASK__, GPIO_Speed_50MHz, GPIO_Mode_Out_PP)

#define DPORT_AFIN_INIT(__PORT__, __MASK__)\
    dport_init(__PORT__, __MASK__, GPIO_Speed_50MHz, GPIO_Mode_AF_OD)

#define DPORT_AFOUT_INIT(__PORT__, __MASK__)\
    dport_init(__PORT__, __MASK__, GPIO_Speed_50MHz, GPIO_Mode_AF_PP)



#define DPORT_SET(__PORT__, __MASK__)\
    (__PORT__)->BSHR = (__MASK__)

#define DPORT_CLR(__PORT__, __MASK__)\
    (__PORT__)->BCR = (__MASK__)

#define DPORT_WRITE(__PORT__, __MASK__, __LEVEL__)\
    if(1 == (__LEVEL__)) {\
    	DPORT_SET(__PORT__, __MASK__);\
    } else {\
    	DPORT_CLR(__PORT__, __MASK__);\
    }\

#define DPORT_GET(__PORT__, __MASK__)\
    (((uint16_t)__PORT__->INDR) & __MASK__)

#endif//DPORT_H
