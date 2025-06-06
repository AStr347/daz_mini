/**
 * event engine module
 */
#define INEVENT_MODULE		0x00000100
#include "inevent_i.h"

/**
 * apply list of events to the event machine
 * @param this		- [inout] new event list
 * @param block		- [in] event block to apply
 */
static void state_change(inevent_t * restrict this,
		         const ine_state_t* restrict ine_st)
{
#ifdef __DEBUG
    for(;NULL == ine_st;){
        __asm__ volatile ("ebreak");
    }
#endif
	const uf8 count = ine_st->count;
	const event_block_t * restrict blocks = ine_st->block;
	
	/* call epilog */
	const inevent_slot_handler epilog = this->slot[EVENT_EPILOG];
	if(epilog){
		epilog();
	}

	/* clear call list */
	memclr(this->slot + 1, sizeof(inevent_slot_handler) * (EVENT_CNT - 1));
	
	for(uf8 sel = 0; count > sel; ++sel){
		const uf8 event = blocks->event;
        if(0 < event && EVENT_CNT > event){
            const inevent_slot_handler handler = blocks->handler;
            this->slot[event] = handler;
        }
        ++blocks;
	};

	/* call prolog */
	const inevent_slot_handler prolog = this->slot[EVENT_PROLOG];
	if(prolog){
		prolog();
	}
}

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * 
 * @return		- true if success
 */
__attribute__((nonnull (1)))
b8 inevent_event(inevent_t * restrict this,
		 const inevent_events_t event)
{
	const struct __packed{
		u8 data;
	} tx_data = {
		.data = event,
	};

	mlock(this->mutex);	
	const b8 result = sfifo8_put(&this->req, (u8*)(&tx_data), 1);
	if(likely(this->waiting)){
		csignal(this->cond);	
	}
	mulock(this->mutex);
	return result;
}

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * @param arg		- [in] argument to pass
 * 
 * @return		- true if success
 */
__attribute__((nonnull (1)))
b8 inevent_event_arg1(inevent_t * restrict this,
		      const inevent_events_t event,
		      const u8 arg)
{
	const struct __packed{
		u8 data;
		u8 arg;
	} tx_data = {
		.data = event | (1 << INEVENT_ARG_POS),
		.arg = arg,
	};

	mlock(this->mutex);	
	const b8 result = sfifo8_put(&this->req, (u8*)(&tx_data), 2);
	if(likely(this->waiting)){
		csignal(this->cond);	
	}	
	mulock(this->mutex);
	return result;	
}

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * @param arg		- [in] argument to pass
 * 
 * @return		- true if success
 */
__attribute__((nonnull (1)))
b8 inevent_event_arg2(inevent_t * restrict this,
		      const inevent_events_t event,
		      const u16 arg)
{
	const struct __packed{
		u8 data;
		u16 arg;
	} tx_data = {
		.data = event| (2 << INEVENT_ARG_POS),
		.arg = arg,
	};

	mlock(this->mutex);	
	const b8 result = sfifo8_put(&this->req, (u8*)(&tx_data), 3);
	if(likely(this->waiting)){
		csignal(this->cond);	
	}	
	mulock(this->mutex);
	return result;
}

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * @param arg		- [in] argument to pass
 * 
 * @return		- true if success
 */
__attribute__((nonnull (1)))
b8 inevent_event_arg4(inevent_t * restrict this,
		      const inevent_events_t event,
		      const u32 arg)
{
	const struct __packed{
		u8 data;
		u32 arg;
	} tx_data = {
		.data = event | (3 << INEVENT_ARG_POS),
		.arg = arg,
	};

	mlock(this->mutex);	
	const b8 result = sfifo8_put(&this->req, (u8*)(&tx_data), 5);
	if(likely(this->waiting)){
		csignal(this->cond);	
	}
	mulock(this->mutex);
	return result;
}

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * @param arg		- [in] argument to pass
 * 
 * @return		- true if success
 */
__attribute__((nonnull (1)))
b8 inevent_event_arg8(inevent_t * restrict this,
		      const inevent_events_t event,
		      const u32 arg0,
		      const u32 arg1)
{
	const struct __packed{
		u8 data;
		u32 arg0;
		u32 arg1;
	} tx_data = {
		.data = event | (4 << INEVENT_ARG_POS),
		.arg0 = arg0,
		.arg1 = arg1,
	};

	mlock(this->mutex);	
	const b8 result = sfifo8_put(&this->req, (u8*)(&tx_data), 9);
	if(likely(this->waiting)){
		csignal(this->cond);
	}
	mulock(this->mutex);
	return result;
}

/**
 * send event to event queue
 * @param this		- [inout] inevent instance
 * @param event		- [in] event to pass
 * 
 * @return		- true if success
 */
__attribute__((nonnull (1, 2)))
b8 inevent_events(inevent_t * restrict this,
				  const inevent_events_t * const events,
				  const u8 cnt)
{
	mlock(this->mutex);	
	const b8 result = sfifo8_put(&this->req, (u8*)events, cnt);
	if(likely(this->waiting)){
		csignal(this->cond);	
	}
	mulock(this->mutex);
	return result;
}

/**
 * handle all inevent data
 * @param this		- [inout] inevent instance
 */
__attribute__((nonnull (1), noreturn))
static
void inevent_routine(inevent_t * restrict this)
{
	static const u8 arg_cnt_translator[5] = {
		[0] = 0,	/* no arg */
		[1] = 1,	/* 1 byte arg */
		[2] = 2,	/* 2 byte arg */
		[3] = 4,	/* 4 byte arg */
		[4] = 8,	/* 4 byte arg0, 4 byte arg1 */
	};
	
	mlock(this->mutex);
	for(;;){
		uf8 rawh;
		/* get new command from fifo */
		const uf16 gcount = sfifo8_get(&this->req, (u8*)(&rawh), 1);
		if(gcount){
			struct __packed{
				u32 arg0;
				u32 arg1;
			} args = {
				.arg0 = 0,
				.arg1 = 0,
			};
			
			const uf8 slot = (rawh >> INEVENT_SLOT_POS) & INEVENT_SLOT_MASK;
			const uf8 argcnt = arg_cnt_translator[(rawh >> INEVENT_ARG_POS) & INEVENT_ARG_MASK];
			const inevent_slot_handler handler = this->slot[slot];

			if(0 < argcnt){
				const uf16 gcount = sfifo8_get(&this->req, (u8*)(&args), argcnt);
				if(unlikely(argcnt != gcount)){
					continue;	
				}
			}
            
			if(NULL != handler){
				mulock(this->mutex);
				handler(args.arg0, args.arg1);
				mlock(this->mutex);
			}
		} else {
			/* wait new event */
			this->waiting = true;
			__unused
			u32 waited = 0;
			cmwait(this->cond, this->mutex, waited);
			this->waiting = false;
		}
	}	
}

/**
 * init inevent system
 * @param this		- [out] inevent instance
 */
__attribute__((nonnull (1)))
void inevent_init(inevent_t * restrict this)
{
	memclr(this, sizeof(inevent_t));
	minit(this->mutex);
	cinit(this->cond);

	this->waiting = false;
	(void)sfifo8_init(&this->req, this->req_data, INEVENT_EVENT_MAX * INEVENT_EVENT_SIZE);
	memset((void*)(this->slot + 1), 0, sizeof(inevent_slot_handler) * (EVENT_CNT - 1));
	this->slot[0] = (inevent_slot_handler)state_change;

	/* don't control creating the threads */
	TaskCreate(this, inevent_routine, INEVENT_STACK_SIZE, INEVENT_PRIORITY);
}
