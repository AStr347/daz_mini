#include "rtc_drv.h"
#include "log.h"

#define RTC_EVENT_PERIOD			((1 << 15) - 1)

#define RTC_EVENTS_CNT	(4)

static const
inevent_events_t rtc_events[RTC_EVENTS_CNT] = {
	[0] = EVENT_TIME_DSEC,
	[1] = EVENT_TIME_HSEC,
	[2] = EVENT_TIME_SEC,
	[3] = EVENT_TIME_MIN,
};

typedef struct {
    inevent_t * in0;
    tdc_t waiter;

    TASK_T(RTC_STACK_SIZE);
} rtc_t;
rtc_t rtc0;

__attribute__((interrupt()))
void RTC_IRQHandler(void){
	const b8 is_sec = RTC_GetITStatus(RTC_IT_SEC) != RESET;
	/* always clean flag */
	RTC_ClearFlag(RTC_FLAG_OW | RTC_FLAG_ALR | RTC_FLAG_SEC);
	if(is_sec) {
		/* Seconds interrupt */
		BaseType_t bogus = 0;
		xEventGroupSetBitsFromISR(rtc0.waiter.cond, 1, &bogus);
    }
	NVIC_ClearPendingIRQ(RTC_IRQn);
}

static
void rtc_routine(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_DeInit();
	PWR_BackupAccessCmd(ENABLE);

	RCC_LSEConfig(RCC_LSE_ON);

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);

	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_EnterConfigMode();

	RTC->CTLRH = RTC_FLAG_SEC;
	RTC->PSCRH = ((RTC_EVENT_PERIOD >> 16) & 0xffff);
	RTC->PSCRL = ((RTC_EVENT_PERIOD >> 0) & 0xffff);
	RTC->DIVH = 0;
	RTC->DIVL = 1;

	/* get now RTC0->cnt */
	u32 posix = rtc_posix_get();
	/* convert and fix if some strange data */
	bcd_time_t rtc_bcd = bcdt_posix2bcd(posix);
	bcdt_fix(&rtc_bcd);
	/* convert back and set RTC0->cnt */
	posix = bcdt_bcd2posix(&rtc_bcd);
	LOGW("new date: %x.%x.%x %x:%x:%x", rtc_bcd.day, rtc_bcd.month, rtc_bcd.year, rtc_bcd.hour, rtc_bcd.min, rtc_bcd.sec);
	LOGW("new posix: %ld", posix);

	RTC->CNTH = ((posix >> 16) & 0xffff);
	RTC->CNTL = ((posix >> 0) & 0xffff);

	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_ExitConfigMode();

	const NVIC_InitTypeDef rtc_nvic = {
		.NVIC_IRQChannel = RTC_IRQn,
		.Priority = 0xf1,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&rtc_nvic);

	u8 dsec = 0;
	u8 sec = 0;
	/* config done.
	 * main rtc loop
	 */
	for(;;){
		/* always dsec event */
		u8 event_cnt = 1;
        if(0 == dsec){
			/* wait sec interrupt */
			__unused
            u32 waitres = 0;
            cwait(rtc0.waiter, waitres);
			//iwait(RTC_IRQ, false);
			/* after interrupt we have 'half sec' and 'sec' event */
			event_cnt += 2;
			if(0 == sec){
				/* add minute event */
				event_cnt++;
				sec = 59;
			} else {
				sec--;
			}
			/** 
			 * put array of events
			 * DSEC, HSEC, SEC, MIN
			 * regulate by event_cnt
			 */
			inevent_events(rtc0.in0, rtc_events, event_cnt);
			dsec = 9;
			continue;
		} else {
			/* sleep less dsec for have posible good irq_rtc(sec) sync */
			msleep(95);
			if(5 == dsec){
				/* add 'half sec' event */
				event_cnt++;
			}
			/** 
			 * put array of events
			 * DSEC, HSEC
			 * regulate by event_cnt
			 */
			inevent_events(rtc0.in0, rtc_events, event_cnt);
			dsec--;
		}
	}
}

void rtc_init(inevent_t * const in0){
	memclr(&rtc0, sizeof(rtc_t));
    rtc0.in0 = in0;
    cinit(rtc0.waiter);

    TaskCreate(&rtc0, rtc_routine, RTC_STACK_SIZE, RTC_PRIORITY);
}

/**
 * get RTC counter value - we interpritate it how posix time value
 */
u32 rtc_posix_get(void)
{
	const u16 hi = RTC->CNTH;
	const u16 lo = RTC->CNTL;

	return ((hi << 16) | lo);
}

/**
 * get bcd time value from RTC
 */
bcd_time_t rtc_bcd_get(void)
{
	const u32 posix = rtc_posix_get();
	return bcdt_posix2bcd(posix);
}

/**
 * set new RTC counter value - we interpritate it how posix time value
 */
void rtc_posix_set(const u32 posix)
{
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_EnterConfigMode();
	
	RTC->CNTH = ((posix >> 16) & 0xffff);
    RTC->CNTL = ((posix >> 0) & 0xffff);

	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_ExitConfigMode();
}

/**
 * set RTC counter by bcd time
 */
void rtc_bcd_set(const bcd_time_t * const bcd)
{
	const u32 posix = bcdt_bcd2posix(bcd);
	rtc_posix_set(posix);
}
