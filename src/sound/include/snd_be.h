#ifndef SND_BE_H
#define SND_BE_H
#include "ia_arch.h"
#include "snd_gen.h"

/* sound backend modes */
typedef enum {
	SND_BE_OFF = 0,
	SND_BE_READY,
    SND_BE_PLAYING,
	SND_BE_MODECOUNT,
} snd_be_mode_t;

typedef b8 (*snd_be_on_t)(void * this);
typedef void (*snd_be_off_t)(void * this);

typedef void (*snd_be_start_t)(void * this, const samplebuffer_t * const buff);
typedef void (*snd_be_stop_t)(void * this);

typedef b8 (*snd_be_wait_t)(void * this);

typedef struct
{
    snd_be_on_t on;
    snd_be_off_t off;
    snd_be_start_t start;
    snd_be_stop_t stop;
    snd_be_wait_t wait;
} snd_be_ft_t;

typedef struct {
    const snd_be_ft_t * ft;
    void * this;
} snd_be_t;

typedef snd_be_t * (*snd_be_init_t)(void * const be, const void * const be_conf);

#ifdef SND_MODULE

#define this_be_on()\
(this->be->ft->on(this->be->this))

#define this_be_off()\
(this->be->ft->off(this->be->this))

#define this_be_start(__BUFF__)\
(this->be->ft->start(this->be->this, __BUFF__))

#define this_be_stop()\
(this->be->ft->stop(this->be->this))

#define this_be_wait()\
(this->be->ft->wait(this->be->this))

#endif

#endif//SND_BE_H
