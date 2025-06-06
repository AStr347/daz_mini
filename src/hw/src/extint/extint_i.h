#ifndef EXTINT_MODULE
#error "must be include just into extint.c"
#endif
#include "extint.h"
#include "log.h"

static const
IRQn_Type extint_nvic_irqs[] = {
    [0] = EXTI0_IRQn,
    [1] = EXTI1_IRQn,
    [2] = EXTI2_IRQn,
    [3] = EXTI3_IRQn,
    [4] = EXTI4_IRQn,
    [5] = EXTI9_5_IRQn,
    [6] = EXTI9_5_IRQn,
    [7] = EXTI9_5_IRQn,
    [8] = EXTI9_5_IRQn,
    [9] = EXTI9_5_IRQn,
    [10] = EXTI15_10_IRQn,
    [11] = EXTI15_10_IRQn,
    [12] = EXTI15_10_IRQn,
    [13] = EXTI15_10_IRQn,
    [14] = EXTI15_10_IRQn,
    [15] = EXTI15_10_IRQn,
};
