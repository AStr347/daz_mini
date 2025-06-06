#ifndef BCD_H
#define BCD_H
#include "ia_arch.h"
/**
 * 8bit bin to bcd conversion, return 0xff if bin to large
 * @param bin8		- input bin data
 * @return bcd8		- output 
 */
extern __attribute__((const)) u8 bin8_to_bcd8(const u8 bin8);

/**
 * 8bit bcd to bin conversion. return 0xff if bcd8 is not bcd
 * @param bcd8		- bcd to convert
 * @return bin8 	- converted to bin bcd value
 */
extern __attribute__((const)) u8 bcd8_to_bin8(const u8 bcd8);

#endif
