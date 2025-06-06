#define BCD_MODULE
#include "bcd_internal.h"

/**
 * 8bit bin to bcd conversion, return 0xff if bin to large
 * @param bin8		- input bin data
 * @return bcd8		- output 
 */
__attribute__((const))
u8 bin8_to_bcd8(const u8 bin8)
{
	return (((bin8 / 10) % 10) << 4) | ((bin8 % 10) << 0);
	/* return __bin8_to_bcd8_list[bin8]; */
}

/**
 * 8bit bcd to bin conversion. return 0xff if bcd8 is not bcd
 * @param bcd8		- bcd to convert
 * @return bin8 	- converted to binary bcd value
 */
__attribute__((const))
u8 bcd8_to_bin8(const u8 bcd8)
{
	return ((((bcd8 >> 4) & 0xf) % 10) * 10) + ((((bcd8 >> 0) & 0xf) % 10) * 1);
	/* return __bcd8_to_bin8_list[bcd8]; */
}
