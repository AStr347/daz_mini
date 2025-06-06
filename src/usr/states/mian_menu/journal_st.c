#define JOURNAL_ST
#define STATE_NAME		journal_st

#include "app.h"

#define JOURNAL_STRING_SIZE     (8)

typedef struct {
	u16 timeout;
	u8 sel;
	u8 max;
	u8 pos;
    u16 reserved;

	struct{
		u8 time_string[5];
		u8 text_string_top[8];
	} draw;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

GS_SCREEN(journal, NULL, NULL)

static void jmp_out(void){
    STATE_JMP(&standby_st);
}

/* draw journal pos
 * @param pos		- position in journal
 */
static
void update()
{
	SDATA->timeout = TIMEOUT;

    const u32 pos = SDATA->sel;

	/* draw only pos that less then count */
	if(pos >= journal.list.cnt){
        return;
    }
    jentry_t entry;
    const b8 get_result = journal_get(&journal, &entry, pos);

    if(false == get_result){
        return;
    }
    /* fill name string */
    //memcpy(SDATA->draw.text_string_top, entry.text, 8);
    
    /* fill the time string */
    /* common time */
    /* binary min/hour */
    const uf min = entry.time.min;
    const uf hour = entry.time.hour;
    /* hours */
    const uf bcd_hour = bin8_to_bcd8(hour);
    const uf bcd_min = bin8_to_bcd8(min);

    char* tsrc = (char*) SDATA->draw.time_string;
    *tsrc++ = '0' + ((bcd_hour >> 4) & 0x0f);
    *tsrc++ = '0' + ((bcd_hour >> 0) & 0x0f);
    *tsrc++ = ':';
    /* minutes */
    *tsrc++ = '0' + ((bcd_min >> 4)&0x0f);
    *tsrc++ = '0' + ((bcd_min >> 0)&0x0f);

    /* fill date string */
    journal_screen.lines[0] = SDATA->draw.text_string_top;
    journal_screen.lens[0] = 8;
    journal_screen.lines[1] = SDATA->draw.time_string;
    journal_screen.lens[1] = 5;
    
    graphic_clear(&graphics);
    G_DRAWSCREEN(journal_screen);
}

static
void journal_inc(void)
{
	if(journal.list.cnt > 1){
		if(SDATA->sel + 1 < journal.list.cnt){
			++SDATA->sel;
		}else{
			SDATA->sel = 0;
		}
		update();
	}
}

static
void journal_dec(void)
{
	if(journal.list.cnt > 1){
		if(SDATA->sel){
			--SDATA->sel;
		}else{
			SDATA->sel = journal.list.cnt - 1;
		}
		update();
	}
}

PROLOG()
{
	graphic_clear(&graphics);
	/* clear the out strings */
	memclr(&SDATA->draw.text_string_top, JOURNAL_STRING_SIZE);
	memclr(&SDATA->draw.time_string, 5);

	/* init locales */
	if(0 < journal.list.cnt){
		SDATA->max = SDATA->sel = journal.list.cnt - 1;
		if(journal.list.cnt > 1){
			SDATA->pos = 0;
		}
		update();
		SDATA->timeout = TIMEOUT;
	}else{
		/* short timeout of empty journal */
		SDATA->timeout = 4;
	}
}

EPILOG() { journal_checked = true; }

ON_ESCAPE_H(const u8 time){
    if(SUPER_LONG_HOLD == time){
        journal_reset(&journal);
        jmp_out();
    }
}

ON_HSEC()
{
	if(SDATA->timeout){
		--SDATA->timeout;
	}else{
		jmp_out();
	}
}

ON_RIGHT_P()
{
	journal_inc();
}

ON_LEFT_P()
{
	journal_dec();
}

#define OUT_STATE_EVENT(BTN) {.event = EVENT_##BTN, .handler = (inevent_slot_handler)jmp_out}

INE_STATE(STATE_NAME, 8)
{
	.count = 8,
    .block[0] = PROLOG_EVENT,
    .block[1] = EPILOG_EVENT,
    .block[2] = HSEC_EVENT,
    .block[3] = ESCAPE_H_EVENT,
    .block[4] = OUT_STATE_EVENT(ESCAPE_R),
    .block[5] = OUT_STATE_EVENT(ENTER_P),
    .block[6] = LEFT_P_EVENT,
    .block[7] = RIGHT_P_EVENT,
};
     
