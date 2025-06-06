#ifndef PULSAR_CATCHER_MODULE
#error should be included to pulsar3 module only
#endif
#include "pulsar_catcher.h"
/* preamble definitions in useconds */
#define PRE_MIN			2400
#define PAIR_MIN		(575)
#define PAIR_MAX		(3000)

/*** pulses lenght and count descriptors ***/
#define PULS3_TOTX_MASK		0xffffffffffff


#define PULS_EXTRA_OFFSET		48	
/* pulsar extra information start */
#define PULS_ALFA_LEN_OFFSET	48	/* alfa len, calc by 100ms + 50ms * len */
#define PULS_ALFA_LEN_MASK      0x3f
/* long part lenth = 2 + mult */
#define PULS_MULT_OFFSET		54	
#define PULS_MULT_MASK          0x03

/* minimum pulsar message length */
#define P3ANY_LEN_MIN           24
#define P3ANY_PCNT_MIN          (P3ANY_LEN_MIN * 2)

#define DPULS_LC_AMASK      ((u64)0xffffffffffffffff)
#define DPULS_DATA_CNT      (1)
#define DPULS_LC_DMASK		0xf
#define DPULS_LC_DPOS		0

