#define GRAPHIC_MODULE
#include "graphic_i.h"

/**
 * execute command by backend
 */
void graphic_com_proc(THIS,
                      const graphci_com_u * const comu)
{
    switch (comu->base.com)
    {
        case GC_OFF:{
        	this_be_off();
        }break;
        case GC_ON:{
        	this_be_on();
        }break;
        case GC_CLEAR:{
        	this_be_clear();
        }break;
        case GC_EXPOSE:{
        	this_be_expose();
        } break;
        case GC_SET_CHAR:{
            const graphci_com_char_t * const com = (graphci_com_char_t*)comu;
            this_be_set_char(com->c, com->x, com->y);
        }break;
        case GC_SET_LINE:{
            const graphci_com_line_t * const com = (graphci_com_line_t*)comu;
            const graphci_com_line_data_t * const src_line = (graphci_com_line_data_t * )com->data;
            this_be_set_line(src_line->str, src_line->len, com->y);
        }break;
        case GC_SET_SCREEN:{
            const graphci_com_screen_t * const com = (graphci_com_screen_t*)comu;
            const u8 * lines[GRAPHIC_BE_ROW_CNT];
            u8 lens[GRAPHIC_BE_ROW_CNT];
            const u8 line_cnt = com->lines_cnt;
            const graphci_com_line_data_t * src_line = (graphci_com_line_data_t * )com->data;
            for(u8 i = 0; i < line_cnt; i++){
            	const u8 len = src_line->len;
                lines[i] = src_line->str;
                lens[i] = len;
                src_line = (graphci_com_line_data_t * )((u8 *)src_line + len + 1);
            }
            this_be_set_screen(lines, lens, line_cnt);
        }break;
        case GC_LOAD_EXT:{
            const graphci_com_loadext_t * const com = (graphci_com_loadext_t*)comu;
            this_be_loadextr(com->pos, com->ext);
        } break;
        default:{
            return;
        } break;
    }
}

#ifdef GRAPHIC_THREAD
static
void graphic_routine(THIS)
{
    mlock(this->mutex);
    const graphic_conf_t * const conf = this->conf;
    this->be = conf->be_init(conf->be, conf->be_conf);
    
    b8 exposed = false;

    for(;;){
        graphci_com_u comu = {
            .base.com = GC_NONE,
        };
        gcom_t com = GC_NONE;
        u16 getcnt = sfifo8_get(&this->comfifo, (u8*)&com, 1);
        comu.base.com = com;
        if(1 == getcnt){
        	exposed = false;
            if(GC_WITHOUT_ARGS_LAST < com){
                u8 siz = 0;
                getcnt = sfifo8_get(&this->comfifo, &siz, 1);
                comu.base.siz = siz;
                if(1 != getcnt){
                    sfifo8_reset(&this->comfifo);
                    continue;
                }
                getcnt = sfifo8_get(&this->comfifo, comu.base.data, siz);
                if(siz != getcnt){
                    sfifo8_reset(&this->comfifo);
                    continue;
                }
            }
            graphic_com_proc(this, &comu);
            exposed = (GC_EXPOSE == com);
        } else {
        	__unused
			u32 waitres = 0;
        	if(exposed){
        		cmwait(this->cond, this->mutex, waitres);
        	} else {
        		cmtwait(this->cond, this->mutex, ms2tick(50), waitres);
        		if(false == waitres){
        			this_be_expose();
        			exposed = true;
        		}
        	}
        }
    }
}
#endif

void graphic_init(THIS, const graphic_conf_t * const conf){
	memclr(this, sizeof(graphic_t));
    GTHREAD_INIT();
}
