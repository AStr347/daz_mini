#ifndef PULSAR_CATCHER_H
#define PULSAR_CATCHER_H
#include "ia_arch.h"
#include "slice.h"
#include "link_legacy_parser.h"

/**
 * return true, if something like pulsar present in seq
 * @param data		- [in] data to analyse
 * @param cnt		- [in] data count
 * 
 * @return 		count of pulsar-like samples	
 */
extern
u16 pulsc_test(const slice_t * restrict data,
               const u16 cnt,
               const u16 need_cnt);

/**
 * parse pulsar data
 * @param dst		- [out] parsed data destination 
 * @param data		- [in] data
 * @param cnt		- [in] datacount
 * 
 * @return		- pulsar any out if success,
 * 			  zero else 
 */
extern
b8 pulsc_parse(ll_data_t* restrict dst,
               const slice_t * restrict data,
               const u16 cnt);

/**
 * make pulsar any data 
 * @param data	- [out] slices out
 * @param size	- [in] slicse size
 * @param src	- [in] data to proceed
 * 
 * @return 	 slices count, or 0 if not converted
 */
extern
u16 pulsc_make(slice_t * restrict dst,
               const u16 size,
               const ll_data_t* restrict src);

#endif//PULSAR_CATCHER_H