#include "app.h"

u8 data_overlay[DATA_OVERLAY_SIZE];

journal_t journal;
b8 journal_checked = false;

void app_init(void)
{
    memclr(data_overlay, DATA_OVERLAY_SIZE);
    STATE_JMP(&boot_st);
}
