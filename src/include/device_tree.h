#ifndef DEVICE_TREE_H
#define DEVICE_TREE_H

#include "ia_arch.h"
#include "board.h"
#include "inevent.h"

#include "dma.h"
#include "extint.h"
#include "adc.h"
#include "timer.h"
#include "pwm.h"
#include "ic.h"
#include "spi.h"

#include "rtc_drv.h"
#include "keypad.h"

#ifdef GRAPHIC
#ifdef BCB0802
#include "bcb0802.h"
#endif
#include "graphic.h"
#endif

#include "log.h"
#include "led_addr.h"

#ifdef NET_LEGACY
#ifdef CMT2300A
#include "cmt2300a.h"
#endif
#ifdef CC110LL
#include "cc110ll.h"
#endif
#include "link_legacy.h"
#include "net_legacy.h"
#endif

#ifdef SOUND
#ifdef PWM_SOUND
#include "pwm_sound.h"
#endif

#ifdef SOFTVOL
#include "softvol.h"
#endif

#ifdef TRANSISTORVOL
#include "transistorvol.h"
#endif

#include "snd_engine.h"
#endif

#ifdef UPOWER
#ifdef UPOWER_CHIP
#include "upower_chip.h"
#endif

#include "upower.h"
#endif

#ifdef SSIGN
#include "ssign.h"
#endif

#endif//DEVICE_TREE_H
