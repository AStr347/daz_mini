/* translate event from various inputs 
 * into linearized calls from event engine 
 */
#ifndef INEVENTS_H
#define INEVENTS_H
#include "ia_arch.h"
#include "sfifo8.h"
#include "inevent_events.h"

#define INEVENT_STACK_SIZE	(BASE_STACK_SIZE)
#define INEVENT_PRIORITY	(BASE_TASK_PRI)
/* maximum events, that can hold */
#define INEVENT_EVENT_MAX	8
#define INEVENT_EVENT_SIZE	8

/**
 * used to handle calls from various inputs in event engine
 * @param generic_arg	- [in] optional 32bits of data, that passed as arg
 * 			  to function call
 */
typedef void (*inevent_slot_handler)(/* const u32 generig_arg */);

/**
 * contain event and it's handler
 * @mem event		- event, on that called handler (0 - list ended)
 * @mem handler		- handler, that called on event
 */
typedef struct __packed{
	u8 event;
	inevent_slot_handler handler;	
} event_block_t;


/**
 * inevent state
 * @mem count		- count of blocks
 * @mem block		- block list
 */
typedef struct __packed{
	u8 count;
	event_block_t block[0];	
} ine_state_t;

/**
 * event entry
 * @mem mutex		- mutex to atomic access to fifo
 * @mem cond		- condition to wake the event engine
 * 
 * @mem req		- fifo for events
 * @mem req_data	- fifo for events data
 * 
 * @mem waiting		- flag, that fifo wait for event
 */
typedef struct {
	tdm_t mutex;
	tdc_t cond;
	
	sfifo8_t req;
	u8 req_data[INEVENT_EVENT_MAX * INEVENT_EVENT_SIZE];
	inevent_slot_handler slot[EVENT_CNT];
	
	volatile b8 waiting;

	TASK_T(INEVENT_STACK_SIZE);
} inevent_t;


/*** function prototypes ***/

/**
 * init inevent system
 * @param this		- [out] inevent instance
 */
extern void inevent_init(inevent_t * restrict this) __attribute__((nonnull (1)));

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * 
 * @return		- true if success
 */
extern b8 inevent_event(inevent_t * restrict this,
			const inevent_events_t event)
			__attribute__((nonnull (1)));

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * @param arg		- [in] argument to pass
 * 
 * @return		- true if success
 */
extern b8 inevent_event_arg1(inevent_t * restrict this,
			     const inevent_events_t event,
			     const u8 arg)
			     __attribute__((nonnull (1)));

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * @param arg		- [in] argument to pass
 * 
 * @return		- true if success
 */
extern b8 inevent_event_arg2(inevent_t * restrict this,
			     const inevent_events_t event,
			     const u16 arg)
			     __attribute__((nonnull (1)));

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * @param arg		- [in] argument to pass
 * 
 * @return		- true if success
 */
extern b8 inevent_event_arg4(inevent_t * restrict this,
			     const inevent_events_t event,
			     const u32 arg)
			     __attribute__((nonnull (1)));

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * @param arg		- [in] argument to pass
 * 
 * @return		- true if success
 */
extern b8 inevent_event_arg8(inevent_t * restrict this,
			     const inevent_events_t event,
			     const u32 arg0,
			     const u32 arg1)
			     __attribute__((nonnull (1)));

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * 
 * @return		- true if success
 */
extern __attribute__((nonnull (1, 2)))
b8 inevent_events(inevent_t * restrict this,
				  const inevent_events_t * const events,
				  const u8 cnt);


#include "inevent_state.h"


#endif

#if defined(DEVICE_TREE_H)
extern inevent_t in0;
#define INE_DEFAULT     (in0)
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(INEVENT_DEVICE_TREE)
#define INEVENT_DEVICE_TREE

inevent_t in0;
#define INEVENT_INIT()		    inevent_init(&in0);

#endif
