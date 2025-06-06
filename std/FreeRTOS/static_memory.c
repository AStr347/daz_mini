/* Commons for static memory allocation */
#include "ia_arch.h"
#include "FreeRTOSConfig.h"

#define IDLE_STACK_SIZE			(configMINIMAL_STACK_SIZE)
#define TIMER_STACK_SIZE		(configMINIMAL_STACK_SIZE)

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[IDLE_STACK_SIZE];
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[TIMER_STACK_SIZE];


/***
 * @Brief set memory for idle task
 */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
								    StackType_t **ppxIdleTaskStackBuffer,
									uint32_t *pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = xIdleStack;
	*pulIdleTaskStackSize = IDLE_STACK_SIZE;
}


/**
 * @brief This is to provide the memory that is used by the RTOS daemon/time task.
 *
 * If configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetTimerTaskMemory() to provide the memory that is
 * used by the RTOS daemon/time task.
 */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = TIMER_STACK_SIZE;
}
