#ifndef MACS_H
#define MACS_H
#include "ia_arch.h"
#include "netlayer_generic.h"


#define MACS_COUNT 50

extern
const u32 macs[MACS_COUNT];

extern
const
char * const names_macs[MACS_COUNT];


extern
b8 macs_set_conf(const u8 index);

extern
u8 macs_search(const u32 id);

#endif//MACS_H