#ifndef APP_H
#define APP_H
#include "ia_arch.h"
#include "board.h"
#include "device_tree.h"
#include "states.h"
#include "text.h"
#include "journal.h"
#include "cfg_storage.h"
#include "alarms.h"
#include "ascii.h"
#include "netlayer_generic.h"
#include "buzzer.h"

#define TIMEOUT			180

extern
journal_t journal;
extern
b8 journal_checked;

extern
void app_init(void);

#define DATA_OVERLAY_SIZE   (32)
extern
u8 data_overlay[DATA_OVERLAY_SIZE];

#define SDATA_DEF static sdata_t * const SDATA = (sdata_t *)data_overlay

typedef struct {
	const u8 * lines[GRAPHIC_BE_ROW_CNT];
	u8 lens[GRAPHIC_BE_ROW_CNT];
} gscreen_t;

#define GP_SCREEN(__NAME__, __LINE0__, __LINE1__)\
gscreen_t __NAME__##_screen = {\
	.lines = {\
		[0] = (u8 *)(__LINE0__),\
		[1] = (u8 *)(__LINE1__),\
	},\
	.lens = {\
		[0] = sizeof(__LINE0__) - 1,\
		[1] = sizeof(__LINE1__) - 1,\
	},\
};

#define GS_SCREEN(__NAME__, __LINE0__, __LINE1__)\
static GP_SCREEN(__NAME__, __LINE0__, __LINE1__)

#define GSC_SCREEN(__NAME__, __LINE0__, __LINE1__)\
static const GP_SCREEN(__NAME__, __LINE0__, __LINE1__)

#define G_DRAWSCREEN(__SCREEN__)\
{\
	graphic_clear(&graphics);\
	graphic_set_screen(&graphics, (const u8 **)(__SCREEN__).lines, (const u8 * const)(__SCREEN__).lens, GRAPHIC_BE_ROW_CNT);\
}

#define G_DRAWLINE(__SCREEN__, __NUM__)\
{\
	graphic_set_line(&graphics, __SCREEN__.lines[__NUM__], __SCREEN__.lens[__NUM__], __NUM__);\
}

#endif//APP_H
