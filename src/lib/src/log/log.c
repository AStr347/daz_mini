#include "log.h"

void log_init(void){
    Delay_Init();
    USART_Printf_Init(115200);

    printf("\n\n\n\r");
    LOGI("SystemClk:%u", (unsigned)SystemCoreClock);
	LOGI("DeviceID: %08x", (unsigned)DBGMCU_GetDEVID());
	LOGI("ChipID: %08x", (unsigned)DBGMCU_GetCHIPID());
	LOGI("RevID: %08x", (unsigned)DBGMCU_GetREVID());
    LOGI("FreeRTOS Kernel Version:%s", tskKERNEL_VERSION_NUMBER);
    LOGW("Build date: %s" __DATE__);
    LOGW("Build time: %s" __TIME__);
}
