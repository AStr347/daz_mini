/**
 * todo:
 * all volume levels should be calculated in runtime at
 * subsystem init by pass arg with accepteble volume levels
 */
#ifndef BUZZER_MODULE
#error should be included to buzzer module only
#endif
#include "device_tree.h"
#include "buzzer.h"
#include "piano.h"
#include "tone.h"

static const u8 volume_list5[5] = {
	0,
	60,
	90,
	140,
	255,
};

#define NOTE_S(I, NOTE, LENGTH) .tones[I] = {.freq = NOTE, .length = LENGTH}, .tones[I + 1] = {.freq = NOTE_P, .length = NOTE_PAUSE_STACCATO}
#define NOTE_L(I, NOTE, LENGTH) .tones[I] = {.freq = NOTE, .length = LENGTH}
/* */

#define MELODY_LEN 4

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_test = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},
	NOTE_L(0, NOTE_C3, NOTE_16),
	NOTE_L(1, NOTE_D3, NOTE_16),
	NOTE_L(2, NOTE_E3, NOTE_16),
	NOTE_L(3, NOTE_P, NOTE_16),
};

#undef MELODY_LEN

#define MELODY_LEN 7

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_baby = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_E3, NOTE_8),
	NOTE_L(1, NOTE_D3, NOTE_8),
	NOTE_L(2, NOTE_B2, NOTE_8),
	NOTE_L(3, NOTE_G2, NOTE_8),
	NOTE_L(4, NOTE_E3, NOTE_4),
	NOTE_L(5, NOTE_C3, NOTE_8),
	NOTE_L(6, NOTE_P, NOTE_16),
};
#undef MELODY_LEN

#define MELODY_LEN 4

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_door = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},
	NOTE_L(0, NOTE_E3, NOTE_4),
	NOTE_L(1, NOTE_C3, NOTE_2),
	NOTE_L(2, NOTE_C3, NOTE_4),
	NOTE_L(3, NOTE_P, NOTE_4),
};
#undef MELODY_LEN

#define MELODY_LEN 8

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_door2 = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},
	NOTE_L(0, NOTE_E3, NOTE_8),
	NOTE_L(1, NOTE_DS3, NOTE_8),
	NOTE_L(2, NOTE_E3, NOTE_8),
	NOTE_L(3, NOTE_B2, NOTE_8),
	NOTE_L(4, NOTE_D3, NOTE_8),
	NOTE_L(5, NOTE_C3, NOTE_8),
	NOTE_L(6, NOTE_A2, NOTE_8),
	NOTE_L(7, NOTE_P, NOTE_8),
};
#undef MELODY_LEN

#define MELODY_LEN 5

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_intercom = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_E3, NOTE_8),
	NOTE_L(1, NOTE_C3, NOTE_8),
	NOTE_L(2, NOTE_D3, NOTE_8),
	NOTE_L(3, NOTE_G2, NOTE_4),
	NOTE_L(4, NOTE_P, NOTE_16),
};
#undef MELODY_LEN

#define MELODY_LEN 7

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_phone_home = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_B2, NOTE_16),
	NOTE_L(1, NOTE_DS3, NOTE_16),
	NOTE_L(2, NOTE_B2, NOTE_16),
	NOTE_L(3, NOTE_DS3, NOTE_16),
	NOTE_L(4, NOTE_B2, NOTE_16),
	NOTE_L(5, NOTE_DS3, NOTE_16),
	NOTE_L(6, NOTE_P, NOTE_16),
};
#undef MELODY_LEN

#define MELODY_LEN 7

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_phone_cell = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_C3, NOTE_16),
	NOTE_L(1, NOTE_D3, NOTE_16),
	NOTE_L(2, NOTE_C3, NOTE_16),
	NOTE_L(3, NOTE_D3, NOTE_16),
	NOTE_L(4, NOTE_C3, NOTE_16),
	NOTE_L(5, NOTE_D3, NOTE_16),
	NOTE_L(6, NOTE_P, NOTE_16),
};
#undef MELODY_LEN

#define MELODY_LEN 14

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_smoke = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_C3, NOTE_16),
	NOTE_L(1, NOTE_P, NOTE_32),
	NOTE_L(2, NOTE_C3, NOTE_16),
	NOTE_L(3, NOTE_P, NOTE_32),
	NOTE_L(4, NOTE_C3, NOTE_16),
	NOTE_L(5, NOTE_P, NOTE_32),
	NOTE_L(6, NOTE_C3, NOTE_16),
	NOTE_L(7, NOTE_P, NOTE_32),
	NOTE_L(8, NOTE_C3, NOTE_16),
	NOTE_L(9, NOTE_P, NOTE_32),
	NOTE_L(10, NOTE_C3, NOTE_16),
	NOTE_L(11, NOTE_P, NOTE_32),
	NOTE_L(12, NOTE_C3, NOTE_16),
	NOTE_L(13, NOTE_P, NOTE_32),
};
#undef MELODY_LEN

#define MELODY_LEN 14

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_gas = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_C3, NOTE_16),
	NOTE_L(1, NOTE_P, NOTE_32),
	NOTE_L(2, NOTE_C3, NOTE_16),
	NOTE_L(3, NOTE_P, NOTE_32),
	NOTE_L(4, NOTE_C3, NOTE_16),
	NOTE_L(5, NOTE_P, NOTE_32),
	NOTE_L(6, NOTE_C3, NOTE_16),
	NOTE_L(7, NOTE_P, NOTE_32),
	NOTE_L(8, NOTE_C3, NOTE_16),
	NOTE_L(9, NOTE_P, NOTE_32),
	NOTE_L(10, NOTE_C3, NOTE_16),
	NOTE_L(11, NOTE_P, NOTE_32),
	NOTE_L(12, NOTE_C3, NOTE_16),
	NOTE_L(13, NOTE_P, NOTE_32),
};
#undef MELODY_LEN

#define MELODY_LEN 7

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_window = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_E3, NOTE_16),
	NOTE_L(1, NOTE_D3, NOTE_16),
	NOTE_L(2, NOTE_E3, NOTE_16),
	NOTE_L(3, NOTE_D3, NOTE_16),
	NOTE_L(4, NOTE_E3, NOTE_16),
	NOTE_L(5, NOTE_D3, NOTE_16),
	NOTE_L(6, NOTE_P, NOTE_16),
};
#undef MELODY_LEN

#define MELODY_LEN 10

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_water = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_C3, NOTE_16),
	NOTE_L(1, NOTE_CS3, NOTE_16),
	NOTE_L(2, NOTE_D3, NOTE_16),
	NOTE_L(3, NOTE_DS3, NOTE_16),
	NOTE_L(4, NOTE_E3, NOTE_16),
	NOTE_L(5, NOTE_DS3, NOTE_16),
	NOTE_L(6, NOTE_D3, NOTE_16),
	NOTE_L(7, NOTE_CS3, NOTE_16),
	NOTE_L(8, NOTE_C3, NOTE_16),
	NOTE_L(9, NOTE_P, NOTE_16),
};
#undef MELODY_LEN

#define MELODY_LEN 6
const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_help = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_C3, NOTE_16),
	NOTE_L(1, NOTE_P, NOTE_32),
	NOTE_L(2, NOTE_C3, NOTE_16),
	NOTE_L(3, NOTE_P, NOTE_32),
	NOTE_L(4, NOTE_C3, NOTE_16),
	NOTE_L(5, NOTE_P, NOTE_16),
};
#undef MELODY_LEN

#define MELODY_LEN 5
const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_on = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_C3, NOTE_16),
	NOTE_L(1, NOTE_F3, NOTE_16),
	NOTE_L(2, NOTE_C4, NOTE_16),
	NOTE_L(3, NOTE_F4, NOTE_16),
	NOTE_L(4, NOTE_C5, NOTE_32),
};
#undef MELODY_LEN

#define MELODY_LEN 5
const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_off = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_L(0, NOTE_C5, NOTE_16),
	NOTE_L(1, NOTE_F4, NOTE_16),
	NOTE_L(2, NOTE_C4, NOTE_16),
	NOTE_L(3, NOTE_F3, NOTE_16),
	NOTE_L(4, NOTE_C3, NOTE_16),
};
#undef MELODY_LEN

#if defined(RUSSIAN_MELODIES)
/*  russian melodies start */

#define MELODY_LEN 50
const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_alarm1 = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_S(0, NOTE_B3, NOTE_4),
	NOTE_S(2, NOTE_A3, NOTE_4),
	NOTE_S(4, NOTE_FS3, NOTE_8),
	NOTE_S(6, NOTE_G3, NOTE_8),
	NOTE_S(8, NOTE_A3, NOTE_4),
	NOTE_S(10, NOTE_FS3, NOTE_8),
	NOTE_S(12, NOTE_G3, NOTE_8),
	NOTE_S(14, NOTE_A3, NOTE_4),
	NOTE_S(16, NOTE_G3, NOTE_8),
	NOTE_S(18, NOTE_FS3, NOTE_8),
	NOTE_S(20, NOTE_E3, NOTE_4),

	NOTE_S(22, NOTE_B3, NOTE_8),
	NOTE_S(24, NOTE_B3, NOTE_8),
	NOTE_S(26, NOTE_A3, NOTE_4),
	NOTE_S(28, NOTE_G3, NOTE_24),
	NOTE_S(30, NOTE_FS3, NOTE_8),
	NOTE_S(32, NOTE_G3, NOTE_8),
	NOTE_S(34, NOTE_A3, NOTE_4),
	NOTE_S(36, NOTE_G3, NOTE_24),
	NOTE_S(38, NOTE_FS3, NOTE_8),
	NOTE_S(40, NOTE_G3, NOTE_8),
	NOTE_S(42, NOTE_A3, NOTE_4),
	NOTE_S(44, NOTE_G3, NOTE_8),
	NOTE_S(46, NOTE_FS3, NOTE_8),
	NOTE_S(48, NOTE_E3, NOTE_4),
};
#undef MELODY_LEN

#define MELODY_LEN 42
const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_alarm2 = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_S(0, NOTE_G3, NOTE_4),
	NOTE_S(2, NOTE_G3, NOTE_4),
	NOTE_S(4, NOTE_D3, NOTE_8),
	NOTE_S(6, NOTE_F3, NOTE_4),
	NOTE_S(8, NOTE_D3, NOTE_8),
	NOTE_S(10, NOTE_DS3, NOTE_8),
	NOTE_S(12, NOTE_C3, NOTE_8),
	NOTE_S(14, NOTE_G3, NOTE_4),
	NOTE_S(16, NOTE_D3, NOTE_8),
	NOTE_S(18, NOTE_AS2, NOTE_4),
	NOTE_S(20, NOTE_C3, NOTE_4),
	NOTE_S(22, NOTE_C3, NOTE_4),
	NOTE_S(24, NOTE_G2, NOTE_24),
	NOTE_S(26, NOTE_A2, NOTE_24),
	NOTE_S(28, NOTE_AS2, NOTE_3),
	NOTE_S(30, NOTE_AS2, NOTE_8),
	NOTE_S(32, NOTE_A2, NOTE_8),
	NOTE_S(34, NOTE_G2, NOTE_8),
	NOTE_S(36, NOTE_A2, NOTE_8),
	NOTE_S(38, NOTE_AS2, NOTE_8),
	NOTE_S(40, NOTE_G2, NOTE_4),
};
#undef MELODY_LEN

#define MELODY_LEN 44

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_alarm3 = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_S(0, NOTE_G3, NOTE_4),
	NOTE_S(2, NOTE_E3, NOTE_8),
	NOTE_S(4, NOTE_E3, NOTE_8),
	NOTE_S(6, NOTE_G3, NOTE_4),
	NOTE_S(8, NOTE_E3, NOTE_8),
	NOTE_S(10, NOTE_E3, NOTE_8),
	NOTE_S(12, NOTE_G3, NOTE_8),
	NOTE_S(14, NOTE_F3, NOTE_8),
	NOTE_S(16, NOTE_E3, NOTE_8),
	NOTE_S(18, NOTE_D3, NOTE_8),
	NOTE_S(20, NOTE_C3, NOTE_2),
	NOTE_S(22, NOTE_A3, NOTE_4),
	NOTE_S(24, NOTE_C4, NOTE_8),
	NOTE_S(26, NOTE_A3, NOTE_8),
	NOTE_S(28, NOTE_G3, NOTE_4),
	NOTE_S(30, NOTE_E3, NOTE_8),
	NOTE_S(32, NOTE_E3, NOTE_8),
	NOTE_S(34, NOTE_G3, NOTE_8),
	NOTE_S(36, NOTE_F3, NOTE_8),
	NOTE_S(38, NOTE_E3, NOTE_8),
	NOTE_S(40, NOTE_D3, NOTE_8),
	NOTE_S(42, NOTE_C3, NOTE_2)
};
#undef MELODY_LEN

#define MELODY_LEN 20
const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_alarm4 = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_S(0, NOTE_D3, NOTE_8),
	NOTE_S(2, NOTE_F3, NOTE_8),
	NOTE_S(4, NOTE_A3, NOTE_8),
	NOTE_S(6, NOTE_F3, NOTE_8),
	NOTE_S(8, NOTE_G3, NOTE_4),
	NOTE_S(10, NOTE_F3, NOTE_8),
	NOTE_S(12, NOTE_E3, NOTE_8),
	NOTE_S(14, NOTE_A3, NOTE_4),
	NOTE_S(16, NOTE_G3, NOTE_4),
	NOTE_S(18, NOTE_D3, NOTE_3),
};
#undef MELODY_LEN

/*  end russian melodies start */

#elif defined(FORING_MELODIES)
#define MELODY_LEN 68

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_alarm1 = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_S(0, NOTE_A2, NOTE_8),
	NOTE_S(2, NOTE_GS2, NOTE_16),
	NOTE_S(4, NOTE_P, NOTE_16),
	NOTE_S(6, NOTE_GS2, NOTE_8),
	NOTE_S(8, NOTE_P, NOTE_8),
	NOTE_S(10, NOTE_A2, NOTE_8),
	NOTE_S(12, NOTE_GS2, NOTE_16),
	NOTE_S(14, NOTE_P, NOTE_16),
	NOTE_S(16, NOTE_GS2, NOTE_8),
	NOTE_S(18, NOTE_P, NOTE_8),
	NOTE_S(20, NOTE_A2, NOTE_8),
	NOTE_S(22, NOTE_GS2, NOTE_16),
	NOTE_S(24, NOTE_P, NOTE_16),
	NOTE_S(26, NOTE_GS2, NOTE_4),
	NOTE_S(28, NOTE_E3, NOTE_6),
	NOTE_S(30, NOTE_P, NOTE_8),
	NOTE_S(32, NOTE_P, NOTE_16),
	NOTE_S(34, NOTE_E3, NOTE_8),
	NOTE_S(36, NOTE_DS3, NOTE_16),
	NOTE_S(38, NOTE_P, NOTE_16),
	NOTE_S(40, NOTE_CS3, NOTE_8),
	NOTE_S(42, NOTE_P, NOTE_8),
	NOTE_S(44, NOTE_CS3, NOTE_8),
	NOTE_S(46, NOTE_B2, NOTE_16),
	NOTE_S(48, NOTE_P, NOTE_16),
	NOTE_S(50, NOTE_A2, NOTE_8),
	NOTE_S(52, NOTE_P, NOTE_8),
	NOTE_S(54, NOTE_A2, NOTE_16),
	NOTE_S(56, NOTE_P, NOTE_32),
	NOTE_S(58, NOTE_GS2, NOTE_16),
	NOTE_S(60, NOTE_P, NOTE_16),
	NOTE_S(62, NOTE_FS2, NOTE_8),
	NOTE_S(64, NOTE_P, NOTE_8),
	NOTE_S(66, NOTE_FS2, NOTE_8),
};
#undef MELODY_LEN

#define MELODY_LEN 40

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_alarm2 = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_S(0, NOTE_DS3, NOTE_4),
	NOTE_S(2, NOTE_F3, NOTE_8),
	NOTE_S(4, NOTE_DS3, NOTE_12), /* NOTE_S(4, NOTE_C3, NOTE_16) */
	NOTE_S(6, NOTE_C3, NOTE_4),
	NOTE_S(8, NOTE_C3, NOTE_4),
	NOTE_S(10, NOTE_C3, NOTE_8), /* NOTE_S(12, NOTE_P, NOTE_12)	 NOTE_S(12, NOTE_P, NOTE_16) */
	NOTE_S(12, NOTE_AS2, NOTE_8), /* NOTE_S(14, NOTE_G2, NOTE_16) */
	NOTE_S(14, NOTE_C3, NOTE_8),
	NOTE_S(16, NOTE_CS3, NOTE_8), /* NOTE_S(18, NOTE_AS2, NOTE_16) */
	NOTE_S(18, NOTE_C3, NOTE_4),
	NOTE_S(20, NOTE_P, NOTE_4),
	NOTE_S(22, NOTE_CS3, NOTE_4),
	NOTE_S(24, NOTE_AS2, NOTE_6),
	NOTE_S(26, NOTE_DS3, NOTE_12), /* NOTE_S(28, NOTE_C3, NOTE_16) */
	NOTE_S(28, NOTE_C3, NOTE_4),
	NOTE_S(30, NOTE_P, NOTE_4),
	NOTE_S(32, NOTE_GS2, NOTE_4),
	NOTE_S(34, NOTE_F2, NOTE_6),
	NOTE_S(36, NOTE_AS2, NOTE_12), /* NOTE_S(38, NOTE_G2, NOTE_16) */
	NOTE_S(38, NOTE_DS2, NOTE_4),
};
#undef MELODY_LEN

#define MELODY_LEN 60

const static struct
{
	sound_t header;
	tone_t tones[MELODY_LEN];
} melody_alarm3 = {
	.header = {.codec = SND_CODEC_TONE, .len = MELODY_LEN},

	NOTE_S(0, NOTE_D2, NOTE_4),
	NOTE_S(2, NOTE_G2, NOTE_4),
	NOTE_S(4, NOTE_G2, NOTE_8),
	NOTE_S(6, NOTE_A2, NOTE_8),
	NOTE_S(8, NOTE_G2, NOTE_8),
	NOTE_S(10, NOTE_FS2, NOTE_8),
	NOTE_S(12, NOTE_E2, NOTE_4),
	NOTE_S(14, NOTE_C2, NOTE_4),
	NOTE_S(16, NOTE_E2, NOTE_4),
	NOTE_S(18, NOTE_A2, NOTE_4),
	NOTE_S(20, NOTE_A2, NOTE_8),
	NOTE_S(22, NOTE_B2, NOTE_8),
	NOTE_S(24, NOTE_A2, NOTE_8),
	NOTE_S(26, NOTE_G2, NOTE_8),
	NOTE_S(28, NOTE_FS2, NOTE_4),
	NOTE_S(30, NOTE_D2, NOTE_4),
	NOTE_S(32, NOTE_D2, NOTE_4),
	NOTE_S(34, NOTE_B2, NOTE_4),
	NOTE_S(36, NOTE_B2, NOTE_8),
	NOTE_S(38, NOTE_C3, NOTE_8),
	NOTE_S(40, NOTE_B2, NOTE_8),
	NOTE_S(42, NOTE_A2, NOTE_8),
	NOTE_S(44, NOTE_G2, NOTE_4),
	NOTE_S(46, NOTE_E2, NOTE_4),
	NOTE_S(48, NOTE_D2, NOTE_8),
	NOTE_S(50, NOTE_D2, NOTE_8),
	NOTE_S(52, NOTE_E2, NOTE_4),
	NOTE_S(54, NOTE_A2, NOTE_4),
	NOTE_S(56, NOTE_FS2, NOTE_4),
	NOTE_S(58, NOTE_G2, NOTE_2)};
#undef MELODY_LEN

#endif

/* melody list */
static const sound_t *const buzzer_melodies[BUZ_ALL_COUNT] = {
	[BUZ_TEST] = (sound_t *)&melody_test,
	[BUZ_BABY] = (sound_t *)&melody_baby,
	[BUZ_DOOR] = (sound_t *)&melody_door,
	[BUZ_DOOR2] = (sound_t *)&melody_door2,
	[BUZ_INTERCOM] = (sound_t *)&melody_intercom,
	[BUZ_PHONE_HOME] = (sound_t *)&melody_phone_home,
	[BUZ_PHONE_CELL] = (sound_t *)&melody_phone_cell,
	[BUZ_SMOKE] = (sound_t *)&melody_smoke,
	[BUZ_WINDOW] = (sound_t *)&melody_window,
	[BUZ_WATER] = (sound_t *)&melody_water,
	[BUZ_HELP] = (sound_t *)&melody_help,
	[BUZ_GAS] = (sound_t *)&melody_gas,
	[BUZ_ALARM1] = (sound_t *)&melody_alarm1,
	[BUZ_ALARM2] = (sound_t *)&melody_alarm2,
	[BUZ_ALARM3] = (sound_t *)&melody_alarm3,

	[BUZ_ON] = (sound_t *)&melody_on,
	[BUZ_OFF] = (sound_t *)&melody_off,
};
