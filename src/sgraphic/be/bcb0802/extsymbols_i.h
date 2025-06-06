#include "ia_arch.h"

#define EXTSYMBIL_LEN	(8)

/*
    '
    '    Custom character definition
    '    ===========================
LCDOUT $FE, $40, $0C, $12, $12, $12, $12, $12, $12, $1E ' 
LCDOUT $FE, $48, $0C, $1E, $1E, $1E, $1E, $1E, $1E, $1E ' 
LCDOUT $FE, $50, $0C, $12, $12, $12, $12, $1E, $1E, $1E ' 
LCDOUT $FE, $58, $0C, $12, $12, $1E, $1E, $1E, $1E, $1E ' 
*/

// 01100
// 10010
// 10010
// 10010
// 10010
// 10010
// 10010
// 11110
static const
u8 __empty_battery[EXTSYMBIL_LEN] = { 0x0C, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x1E, };

// 01100
// 10010
// 10010
// 10010
// 10010
// 11110
// 11110
// 11110
static const
u8 __low_battery[EXTSYMBIL_LEN] = { 0x0C, 0x12, 0x12, 0x12, 0x12, 0x1E, 0x1E, 0x1E, };

// 01100
// 10010
// 10010
// 11110
// 11110
// 11110
// 11110
// 11110
static const
u8 __mid_battery[EXTSYMBIL_LEN] = { 0x0C, 0x12, 0x12, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, };

// 01100
// 11110
// 11110
// 11110
// 11110
// 11110
// 11110
// 11110
static const
u8 __full_battery[EXTSYMBIL_LEN] = { 0x0C, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, };


static const
u8 * const __extsymbs[BCB0802_EXTS_CNT] = {
    [BCB0802_EXTS_BAT_EMPTY] = __empty_battery,
    [BCB0802_EXTS_BAT_LOW] = __low_battery,
    [BCB0802_EXTS_BAT_MID] = __mid_battery,
    [BCB0802_EXTS_BAT_FULL] = __full_battery,
};

