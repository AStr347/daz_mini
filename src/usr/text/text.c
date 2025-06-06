#include "text.h"

const u8 * const ru_text[TXT_COUNT] = {
	[TXT_EMPTY] = "        ",

	[TXT_SUN_S] = "ВС",
	[TXT_MON_S] = "ПН",
	[TXT_TUE_S] = "ВТ",
	[TXT_WED_S] = "СР",
	[TXT_THU_S] = "ЧТ",
	[TXT_FRI_S] = "ПТ",
	[TXT_SAT_S] = "СБ",

	[TXT_MM] = "Глав. М.",
	[TXT_MM_JOURNAL] = "Журнал  ",
	[TXT_MM_ALARM] = "Будильн.",
	[TXT_MM_TIME] = "Время   ",
	[TXT_MM_DATE] = "Дата    ",
	[TXT_MM_VOL] = "Громк.  ",
	[TXT_MM_NIGHT_COLOR] = "Н. цвет ",
	[TXT_MM_LONG_SIGNAL] = "В. сигн.",
	[TXT_MM_MODE] = "Режим   ",
	[TXT_MM_NET] = "Датчики ",

	[TXT_AM_TIME] = "Время   ",
	[TXT_AM_MELODY] = "Мелодия ",
	[TXT_AM_SNOOZE] = "Отсрочка",

	[TXT_AM_EN] = "Вкл.    ",
	[TXT_AM_DIS] = "Выкл.   ",

	[TXT_NM_CONNECT] = "Сопряж. ",
	[TXT_NM_EDIT] = "Редакт. ",

	[TXT_NMC_SEC] = "Сек.",
	[TXT_NMC_CNT] = "Сопр.",

	[TXT_NMED_CHOOSE] = "Выбор   ",

	[TXT_NMED_NAME0] = "Имя Вер ",
	[TXT_NMED_NAME1] = "Имя Низ ",
	[TXT_NMED_COLOR] = "Цвет    ",
	[TXT_NMED_MELODY] = "Мелодия ",
	[TXT_NMED_VIBRO] = "Вибрация",

	[TXT_ALARM] = "БУДИЛЬН.",

	/* names for signals - each string 15 symbols */
	[TXT_RC_BABYCRY]        	= "ПЛАЧ\0\0\0\0" "РЕБЕНКА\0",	/* ok091219 */
	[TXT_RC_BABYCRY_HOPE_RF]  	= "ПЛАЧ\0\0\0\0" "РЕБЕНКА\0", 	/* new 160522 */
	[TXT_RC_WAKE_SIGNAL]  		= "БУДИЛЬН." "\0\0\0\0\0\0\0\0",	/* ok091219 */
	[TXT_RC_BUTTON]         	= "ДВЕРНОЙ\0" "ЗВОНОК\0\0",		/* ok091219 */
	[TXT_SOS_BUTTON]        	= "СОС\0\0\0\0\0" "БРЕЛОК\0\0",	/* ok091219 */
	[TXT_RC_CELLPHONE_HOPE_RF]  = "СОТОВЫЙ\0" "ТЕЛЕФОН\0",
	[TXT_RC_TELEPHON]       	= "ТЕЛЕФОН\0" "\0\0\0\0\0\0\0\0",		/* ok091219 */
	[TXT_GSM_GATEWAY_COM3]  	= "ОХРАНА\0\0" "ВКЛЮЧЕНА",	/* ok091219 */
	[TXT_RC_DOORPHONE]      	= "ДОМОФОН\0" "\0\0\0\0\0\0\0\0",		/* ok091219 */
	[TXT_FIRE_ALARM]        	= "ПОЖАРНАЯ" "ТРЕВОГА\0",		/* ok091219 */
	[TXT_MOTION_SENSOR]     	= "ДВИЖЕНИЕ" "\0\0\0\0\0\0\0\0",		/* ok091219 */
	[TXT_RC_CALLHELP]       	= "НУЖНА\0\0\0" "ПОМОЩЬ\0\0",	/* ok091219 */
	[TXT_RC_CALL]           	= "КНОПКА\0\0" "ВЫЗОВА\0\0",		/* ok091219 */
	[TXT_DOOR_WIND_OPENED]  	= "ОТКРЫТА\0" "ДВЕРЬ\0\0\0",       /* ok091219 */
	[TXT_GASLEAK]           	= "ГАЗ\0\0\0\0\0" "\0\0\0\0\0\0\0\0",		/* ok091219 */
	[TXT_WATERLEAK]         	= "ВОДА\0\0\0\0" "\0\0\0\0\0\0\0",         /* ok091219 */
	[TXT_CELLPHONE]         	= "СОТОВЫЙ\0" "ТЕЛЕФОН\0",
	[TXT_WAKE_SIGNAL]       	= "БУДИЛЬН." "\0\0\0\0\0\0\0\0",


	[TXT_BLUE] = "синий   ",
	[TXT_GREEN] = "зеленый ",
	[TXT_CYAN] = "голубой ",
	[TXT_RED] = "красный ",
	[TXT_MAGANTA] = "розовый ",
	[TXT_YELLOW] = "желтый  ",
	[TXT_ORANGE] = "оранж.  ",
	[TXT_WHITE] = "белый   ",
	[TXT_BLACK] = "черный  ",

	[TXT_IS_NONE] = "  Выкл. ",
	[TXT_IS_LED] = "Св.     ",
	[TXT_IS_SOUND] = "   Зв.  ",
	[TXT_IS_SOUND_LED] = "Св.Зв.  ",
	[TXT_IS_VIBRO] = "      В.",
	[TXT_IS_VIBRO_LED] = "Св.   В.",
	[TXT_IS_VIBRO_SOUND] = "   Зв.В.",
	[TXT_IS_ALL] = "Св.Зв.В.",
};


