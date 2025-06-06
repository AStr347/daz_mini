#define NET_CONNECT_ST
/*
 * NET connect state
 */

#define STATE_NAME		connect_st

#include "app.h"

/* if this macro is more then zero it will
 * try to connect N bogus sensors to device
 * at state start.
 * N is macro count
 */
#define TEST_MODE	0
/* test start is start address of bogus devices,
 * that added
 */
#define TEST_START	0x1000

/**
 * defines light emitting for not connected yet
 * in this session state
 */
#define CON_MODE	         LED_ON
#define CON_COLOR	         BLUE

/**
 * defines light emitting for one and more connected
 * in this sesseion state  
 */
#define NCON_MODE           LED_ON
#define NCON_COLOR_1		BLUE
#define NCON_COLOR_2		RED

/* Pulsar3 have not device code in payload
 * and we will put this device data to make easier using
 * connection
 */
static const
devinfo_t ind_pulsar3_default = {
    .id = 0x0,
    .name = TXT_RC_CALL,
    .color = WHITE_P,
    .melody = 0,
    .vibro = 0,
};

typedef struct {
	devinfo_t* info_sel;
	u16 timeout;
	b8 blinking;
	b8 new_device;

    u32 count;
}state_data_t;
#define SDATA   ((state_data_t*)data_overlay)

static u8 timeout_str[8] 	= "TTTT XXX";
static u8 count_str[8] 		= "CCCC XXX";

GS_SCREEN(connect, timeout_str, count_str)

/**
 * here we draw disconnect window
 */
static
void update(void)
{
	{
		u8 * dst = timeout_str;
		const u8 * src = TXT(TXT_NMC_SEC);
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = ' ';
		ascii_u32_to_str(dst, (SDATA->timeout >> 1), 3);
	}

	{
		u8 * dst = count_str;
		const u8 * src = TXT(TXT_NMC_CNT);
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = ' ';
		ascii_u32_to_str(dst, SDATA->count, 3);
	}

	G_DRAWSCREEN(connect_screen);
}

/**
 * Start to blinking red or blue color
 */
static
void led_red_blue(void)
{
	if(true == SDATA->blinking){
		led_addr_set(NCON_COLOR_1);
	}else{
        led_addr_set(NCON_COLOR_2);
	}
    SDATA->blinking = !SDATA->blinking;
}

PROLOG()
{
    SDATA->info_sel = NULL;
	SDATA->timeout = 600;
	SDATA->blinking = false;
	SDATA->new_device = false;
	SDATA->count = conf_gnet_get(net0, &config);

    /* pulsar3 devices commonly connects transmitters one by one */
	net0_paring_on(1);
    update();
}

EPILOG()
{
	net0_normal();
	led_addr_off();
	net0_flush();
	cfg_storage_sync();
}

ON_HSEC()
{
    led_red_blue();

	if(SDATA->timeout){
		--SDATA->timeout;
		update();
	}else{
		STATE_JMP(&standby_st);
	}
}

ON_NETL_INFO()
{
	/* if we got message from net legacy, it should be
	 * message from new device
	 * or message from already connected device
	 * 
	 * in both situations we must leave the state
	 * 
	 * now(16.11.18) all data get the rx buffer,
	 * connect data marked as NETL_CONN
	 */

	net_rx_t message;
	/* get first message and goto edit */
	const b8 read_res = net0_read(&message);
	if(read_res){
		const u64 mac = message.id;
		/* check this mac in our storage */
		if(false == conf_net_present(&config, mac)){
			devinfo_t info = ind_pulsar3_default;
            info.id = mac;
            
			SDATA->new_device = true;
			/* go edit this device */
			if(conf_net_add(&config, &info)){
				SDATA->info_sel = conf_net_notify_get(&config, mac);
				STATE_JMP(&net_edit_dev_st);
			}
		}else{
			/* got this mac in config -> edit it */
			SDATA->new_device = false;
            devinfo_t * dev = conf_net_notify_get(&config, mac);
            if(NULL != dev){
                SDATA->info_sel = dev;
                STATE_JMP(&net_edit_dev_st);
            }
		}
	}
}

ON_ESCAPE_P()
{
	STATE_JMP(&net_menu_st);
}

ON_ENTER_P()
{
	STATE_JMP(&net_menu_st);
}

INE_STATE(STATE_NAME, 6)
{
	.count = 6,
    .block[0] = PROLOG_EVENT,
    .block[1] = EPILOG_EVENT,
    .block[2] = HSEC_EVENT,
    .block[3] = ESCAPE_P_EVENT,
    .block[4] = ENTER_P_EVENT,
    .block[5] = NETL_INFO_EVENT,
};
