#ifndef JOURNAL_NEW_H
#define JOURNAL_NEW_H
#include "ia_arch.h"
#include "rtc_drv.h"
#include "ia_list.h"
//#include "indication.h"

#define JOURNAL_SIZE		(16)
#define JOURNAL_TIMEOUT_H	(24)

/**
 * one journal string
 * @mem time	- time of event
 * @mem text	- event text
 */
typedef struct {
	bcd_time_t time;
	u8 text;
	/* todo: size of text should be done through notify */
	/* char text[16]; */
} jentry_t;

typedef struct {
	list_t list;
	u32 cache;
	jentry_t data[JOURNAL_SIZE];
} journal_t;

/**
 * init journal with default params
 * @param jrn		- journal instance
 */
void journal_init(journal_t * restrict jrn);

/**
 * clear journal
 * @param jrn		- journal instance
 */
void journal_reset(journal_t * restrict jrn);

/**
 * get from journal entry with index
 * @param jrn		- journal instance
 * @param index		- journal index
 * @return 		- true on success,
 * 			  false else
 */
b8 journal_get(journal_t * restrict jrn,
	       jentry_t * restrict entry,
	       const u32 index);

/**
 * put journal entry to journal
 * @param jrn		- journal instance
 * @param entry		- entry to put
 * @return 		- true if success,
 * 			  false else
 */
b8 journal_add(journal_t * restrict jrn,
	       const jentry_t * restrict entry);

/**
 * replace entry with num with new one
 * @param jrn		- journal instance 
 * @param entry		- new entry
 * @param num		- num
 * @return 		- true if success, false else
 */
b8 journal_replace(journal_t * restrict jrn,
		   const jentry_t * restrict entry,
		   const uf num);

#endif
