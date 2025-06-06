#define ASCII_MODULE
#include "ascii_internal.h"

/**
 * one char to value by hex
 * @param c
 * @return '0'-'9' then 0-9; 'a'-'f' then 10-15; 'A'-'F' then 10-15; else  0
 */
static
u8 hex2bin(const char c)
{
	u8 result = 0;
	if(c >= 'a' && c <= 'f'){
		result = c - 'a' + 10;
	}else if(c >= 'A' && c <= 'F'){
		result = c - 'A' + 10;
	}else if(c >= '0' && c <= '9'){
		result = c - '0';
	}
	return result;
}

/**
 * translate hex string to u64
 * @param str -  hex string without "0x"
 * @param len - lenght of hex string (u8 - 2, u16 - 4, u32 - 8, u64 - 16)
 * @return translated value
 */
u64 ascii_hex_to_u64(const char* str, const u8 len)
{
	u64 result = 0;
	u64 hexo = 1;
	for(u8 i = len; i; i--){
		char c = hex2bin(str[i - 1]);
		result += c * hexo;
		hexo *= 16;
	}
	return result;
}

/**
 * 4bits to hex char
 * @param b
 * @return 0x0-0xf then '0' - 'f' else '0'
 */
__attribute__((const))
static
char bin2hex(const u8 b)
{
	static const char converter[] = "0123456789ABCDEF";
	char result = '0';
	if(b <= 0xf){
		result = converter[b];
	}
	return result;
}

/**
 * u64 to hex str without "0x"
 * @param str - string for fill
 * @param data - value
 * @param len - len of string (u8 - 2, u16 - 4, u32 - 8, u64 - 16)
 */
char* ascii_u64_to_hex(char* str,
                       const u64 data,
                       const u8 len)
{
	static const u8 moves[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60};
	u8 index = 0;
	for(u8 i = len; i; i--, index++){
		u8 b4 = data >> moves[index] & 0xf;
		char h = bin2hex(b4);
		str[i - 1] = h;
	}
    return str + len;
}

/**
 * u32 to dec str
 * @param str - string for fill
 * @param data - value
 * @param len - len of string (u8 - 3, u16 - 5. u32 - 10)
 */
char* ascii_u32_to_str(char* str,
                       const u32 data,
                       const u8 len)
{
    u32 val = data;
    for(u8 i = len; i; i--){
		str[i - 1] = '0' + val % 10;
		val /= 10;
	}
    return str + len;
}
