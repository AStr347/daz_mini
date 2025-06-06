#define JOURNAL_NEW_MODULE
#include "journal_i.h"

/**
 * init journal with default params
 * @param jrn		- journal instance
 */
void journal_init(journal_t * restrict jrn)
{
	memclr(jrn, sizeof(journal_t));
	list_init((list_t*)jrn, JOURNAL_SIZE, sizeof (jentry_t));
}

/**
 * clear journal
 * @param jrn		- journal instance
 */
void journal_reset(journal_t * restrict jrn)
{
	list_reset((list_t*)jrn);
}

/**
 * get from journal entry with index
 * @param jrn		- journal instance
 * @param index		- journal index
 * @return 		- true on success,
 * 			  false else
 */
b8 journal_get(journal_t * restrict jrn,
	       jentry_t * restrict entry,
	       const u32 index)
{
	const s32 result = list_get((list_t*)jrn, entry, sizeof (jentry_t), index);

	return result >= 0 ? true : false;
}

/**
 * put journal entry to journal
 * @param jrn		- journal instance
 * @param entry		- entry to put
 * @return 		- true if success,
 * 			  false else
 */
b8 journal_add(journal_t * restrict jrn,
	       const jentry_t * restrict entry)
{
	const s32 result = list_add((list_t*)jrn, entry);

	return result >= 0 ? true : false;
}

/**
 * replace entry with num with new one
 * @param jrn		- journal instance 
 * @param entry		- new entry
 * @param num		- num
 * @return 		- true if success, false else
 */
b8 journal_replace(journal_t * restrict jrn,
		   const jentry_t * restrict entry,
		   const uf num)
{
	const s32 result = list_put((list_t*)jrn, entry, num); 

	return result >= 0 ? true : false;
}
