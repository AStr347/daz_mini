#ifndef ASCII_H
#define ASCII_H
#define ASCII_LIB_VER		0x00000002
#include "ia_arch.h"

/**
 * translate hex string to u64
 * @param str -  hex string without "0x"
 * @param len - lenght of hex string
 * @return translated value
 */
extern u64 ascii_hex_to_u64(const char* str, const u8 len);

/**
 * u64 to hex str without "0x"
 * @param str - string for fill
 * @param data - value
 * @param len - len of string (u8 - 2, u16 - 4, u32 - 8, u64 - 16)
 */
extern char* ascii_u64_to_hex(char* str, const u64 data, const u8 len);


/**
 * u32 to dec str
 * @param str - string for fill
 * @param data - value
 * @param len - len of string (u8 - 3, u16 - 5. u32 - 10)
 */
extern char* ascii_u32_to_str(char* str, const u32 data, const u8 len);

#endif
