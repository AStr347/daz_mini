#include "dport.h"

void dport_init(DPORT port,
                const u16 mask,
                const GPIOSpeed_TypeDef speed,
                const GPIOMode_TypeDef mode)
{
    const GPIO_InitTypeDef init_struct = {
        .GPIO_Pin = mask,
        .GPIO_Speed = speed,
        .GPIO_Mode = mode,
    };
    GPIO_Init(port, &init_struct);
}
