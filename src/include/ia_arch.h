#ifndef IA_ARCH_H
#define IA_ARCH_H
#include "board.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
//#include "unistd.h"

#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define BASE_STACK_SIZE 	(192)

typedef uint8_t u8;
typedef int8_t s8;
typedef uint32_t uf8;
typedef int32_t sf8;

typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t uf16;
typedef int32_t sf16;

typedef uint32_t u32;
typedef int32_t s32;
typedef uint32_t uf32;
typedef int32_t sf32;

typedef uint64_t u64;
typedef int64_t s64;
typedef uint64_t uf64;
typedef int64_t sf64;
typedef bool b8;

typedef unsigned int uf;
typedef int sf;

typedef struct s_u8_t {
    u8 len;
    void * ptr;
} s_u8;

typedef struct __attribute__((aligned))
{
	u8 u8_0;
	unsigned: 24;
	u8 u8_1;
	unsigned: 24;
}
u8_u8;

typedef struct __attribute__((aligned))
{
	b8 b8;
	unsigned: 24;
	u8 u8;
	unsigned: 24;
}
b8_u8;

typedef struct __attribute__((aligned))
{
	b8 b8;
	unsigned: 24;
	u16 u16;
	unsigned: 16;
}
b8_u16;

typedef struct __attribute__((aligned))
{
	b8 b8;
	unsigned: 24;
	u32 u32;
}
b8_u32;

typedef struct __attribute__((aligned))
{
	u32 u32_0;
	u32 u32_1;
}
u32_u32;

typedef struct __attribute__((aligned))
{
	u16 u16_0;
	u16 u16_1;
}
u16_u16;

typedef struct __attribute__((aligned))
{
	u32 u32;
	u32 u8;
}
u32_u8;

typedef struct __attribute__((aligned))
{
	u16 u16;
	unsigned:16;
	u32 u32;
}
u16_u32;

#define likely(x)           __builtin_expect((_Bool)(x),true)
#define unlikely(x)         __builtin_expect((_Bool)(x),false)
#define clz(x)				__builtin_clz((x))

/* freertos alias */
#define BASE_TASK_PRI	(4)
#if (1 == configSUPPORT_STATIC_ALLOCATION)
#define TASK_T(__STACK_SIZE__)\
	struct {\
		TaskHandle_t handle;\
		StaticTask_t taskbuffer;\
		StackType_t stack[__STACK_SIZE__];\
	} task

#define TaskCreate(__THIS__, __ROUTINE__, __STACK_SIZ__, __PRI__)\
{\
	(__THIS__)->task.handle = xTaskCreateStatic((TaskFunction_t) __ROUTINE__,\
												#__ROUTINE__,\
												__STACK_SIZ__,\
												(__THIS__),\
												__PRI__,\
												(__THIS__)->task.stack,\
												&((__THIS__)->task.taskbuffer));\
}
#else
#define TASK_T(__STACK_SIZE__)\
	struct {\
		TaskHandle_t handle;\
		StaticTask_t taskbuffer;\
	} task

#define TaskCreate(__THIS__, __ROUTINE__, __STACK_SIZ__, __PRI__)\
{\
	(void)xTaskCreate((TaskFunction_t) __ROUTINE__,\
					  #__ROUTINE__,\
					  __STACK_SIZ__,\
					  (__THIS__),\
					  __PRI__,\
					  &(__THIS__)->task.handle);\
}
#endif

#define ms2tick(__MS__)     ((__MS__) * portTICK_PERIOD_MS)
#define us2tick(__US__)     ((__US__ + 1) / portTICK_PERIOD_US)

/**
 * sleep us, but align to 50us
 */
#define usleep(__US__)\
	vTaskDelay(us2tick(__US__))

#define msleep(__MS__)\
    vTaskDelay(ms2tick(__MS__))

#if (1 == configSUPPORT_STATIC_ALLOCATION)
typedef struct {
	SemaphoreHandle_t mutex;
	StaticSemaphore_t buff;
} tdm_t;
#define minit(__MUTEX__)    (__MUTEX__).mutex = xSemaphoreCreateMutexStatic(&(__MUTEX__).buff)
#else
typedef struct {
	SemaphoreHandle_t mutex;
} tdm_t;
#define minit(__MUTEX__)    (__MUTEX__).mutex = xSemaphoreCreateMutex()
#endif
#define mlock(__MUTEX__)    xSemaphoreTake((__MUTEX__).mutex, portMAX_DELAY)
#define mulock(__MUTEX__)   xSemaphoreGive((__MUTEX__).mutex)

#if (1 == configSUPPORT_STATIC_ALLOCATION)
typedef struct {
	EventGroupHandle_t cond;
	StaticEventGroup_t buff;
} tdc_t;
#define cinit(__COND__) 	(__COND__).cond = xEventGroupCreateStatic(&(__COND__).buff)
#else
typedef struct {
	EventGroupHandle_t cond;
} tdc_t;
#define cinit(__COND__) 	(__COND__).cond = xEventGroupCreate()
#endif
#define csignal(__COND__)   xEventGroupSetBits((__COND__).cond, 1)
#define ctwait(__COND__, __TIME__, __RES__)\
{\
    xEventGroupClearBits((__COND__).cond, 1);\
    __RES__ = xEventGroupWaitBits((__COND__).cond, 1, true, false, __TIME__);\
}

#define cwait(__COND__, __RES__)     ctwait(__COND__, portMAX_DELAY, __RES__)
#define cmwait(__COND__, __MUTEX__, __RES__)\
{\
    mulock(__MUTEX__);\
    cwait(__COND__, __RES__);\
    mlock(__MUTEX__);\
}
#define cmtwait(__COND__, __MUTEX__, __TIME__, __RES__)\
{\
    mulock(__MUTEX__);\
    ctwait(__COND__, __TIME__, __RES__);\
    mlock(__MUTEX__);\
}

#define memclr(__PTR__, __SIZ__)    bzero(__PTR__, __SIZ__)

#endif//IA_ARCH_H
