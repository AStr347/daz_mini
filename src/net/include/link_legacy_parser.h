#ifndef LINK_LEGACY_PARSER_H
#define LINK_LEGACY_PARSER_H
#include "ia_arch.h"
#include "slice.h"

/**
 * link legacy data
 * @mem id	- legacy data identifier
 * @mem data	- extra data
 */
typedef struct {
	u64 id;
	u32 data;
} ll_data_t;

#define LLLEN_DATA_CNT_POS    (24)

/* payload length */
#define LLLEN_OFFSET		56
#define LLLEN_MASK			0x3F
/* base level */
#define LLTYPE_OFFSET		62
#define LLTYPE_MASK			0x03

/* encoding container */
typedef enum enc_type {
	/* must be in this sort because esp have type checks by this values */
    llenc_puls,     /* pulsar */
	llenc_count,
	llenc_kaku,		/* klikaanklikuit */
	llenc_kaku_old,	/* meteostation */
    llenc_real_cnt,        
    llenc_last = llenc_count - 1,
} llenc_type_t;

/**
 * parse various legacy data
 * @param dst		- [out] recceive data 
 * @param data		- [in] data
 * @param cnt		- [in] datacount
 * 
 * @return		- pulsar any out if success,
 * 			  zero else 
 */
typedef b8 (*legacy_parse_t)(ll_data_t* restrict dst,
                             const slice_t * restrict data,
                             const u16 cnt);

/**
 * test presence of legacy data
 * @param data		- [in] data
 * @param cnt		- [in] datacount
 * 
 * @return		- count of slices, that looks like legacy signal
 */
typedef u16 (*legacy_test_t)(const slice_t * restrict data,
                             const u16 cnt,
                             const u16 need_cnt);

/**
 * make varios legacy data 
 * @param data	- [out] slices out
 * @param size	- [in] slicse size
 * @param src	- [in] legacy data
 * 
 * @return 	 slices count, or 0 if not converted
 */
typedef u16 (*legacy_make_t)(slice_t * restrict dst,
                             const u16 size,
                             const ll_data_t* restrict src);

#endif//LINK_LEGACY_PARSER_H