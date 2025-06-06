#include "text.h"

const u8 * const ru_text[TXT_COUNT] = {
	[TXT_EMPTY] = "        ",

	[TXT_SUN_S] = "��",
	[TXT_MON_S] = "��",
	[TXT_TUE_S] = "��",
	[TXT_WED_S] = "��",
	[TXT_THU_S] = "��",
	[TXT_FRI_S] = "��",
	[TXT_SAT_S] = "��",

	[TXT_MM] = "����. �.",
	[TXT_MM_JOURNAL] = "������  ",
	[TXT_MM_ALARM] = "�������.",
	[TXT_MM_TIME] = "�����   ",
	[TXT_MM_DATE] = "����    ",
	[TXT_MM_VOL] = "�����.  ",
	[TXT_MM_NIGHT_COLOR] = "�. ���� ",
	[TXT_MM_LONG_SIGNAL] = "�. ����.",
	[TXT_MM_MODE] = "�����   ",
	[TXT_MM_NET] = "������� ",

	[TXT_AM_TIME] = "�����   ",
	[TXT_AM_MELODY] = "������� ",
	[TXT_AM_SNOOZE] = "��������",

	[TXT_AM_EN] = "���.    ",
	[TXT_AM_DIS] = "����.   ",

	[TXT_NM_CONNECT] = "������. ",
	[TXT_NM_EDIT] = "������. ",

	[TXT_NMC_SEC] = "���.",
	[TXT_NMC_CNT] = "����.",

	[TXT_NMED_CHOOSE] = "�����   ",

	[TXT_NMED_NAME0] = "��� ��� ",
	[TXT_NMED_NAME1] = "��� ��� ",
	[TXT_NMED_COLOR] = "����    ",
	[TXT_NMED_MELODY] = "������� ",
	[TXT_NMED_VIBRO] = "��������",

	[TXT_ALARM] = "�������.",

	/* names for signals - each string 15 symbols */
	[TXT_RC_BABYCRY]        	= "����\0\0\0\0" "�������\0",	/* ok091219 */
	[TXT_RC_BABYCRY_HOPE_RF]  	= "����\0\0\0\0" "�������\0", 	/* new 160522 */
	[TXT_RC_WAKE_SIGNAL]  		= "�������." "\0\0\0\0\0\0\0\0",	/* ok091219 */
	[TXT_RC_BUTTON]         	= "�������\0" "������\0\0",		/* ok091219 */
	[TXT_SOS_BUTTON]        	= "���\0\0\0\0\0" "������\0\0",	/* ok091219 */
	[TXT_RC_CELLPHONE_HOPE_RF]  = "�������\0" "�������\0",
	[TXT_RC_TELEPHON]       	= "�������\0" "\0\0\0\0\0\0\0\0",		/* ok091219 */
	[TXT_GSM_GATEWAY_COM3]  	= "������\0\0" "��������",	/* ok091219 */
	[TXT_RC_DOORPHONE]      	= "�������\0" "\0\0\0\0\0\0\0\0",		/* ok091219 */
	[TXT_FIRE_ALARM]        	= "��������" "�������\0",		/* ok091219 */
	[TXT_MOTION_SENSOR]     	= "��������" "\0\0\0\0\0\0\0\0",		/* ok091219 */
	[TXT_RC_CALLHELP]       	= "�����\0\0\0" "������\0\0",	/* ok091219 */
	[TXT_RC_CALL]           	= "������\0\0" "������\0\0",		/* ok091219 */
	[TXT_DOOR_WIND_OPENED]  	= "�������\0" "�����\0\0\0",       /* ok091219 */
	[TXT_GASLEAK]           	= "���\0\0\0\0\0" "\0\0\0\0\0\0\0\0",		/* ok091219 */
	[TXT_WATERLEAK]         	= "����\0\0\0\0" "\0\0\0\0\0\0\0",         /* ok091219 */
	[TXT_CELLPHONE]         	= "�������\0" "�������\0",
	[TXT_WAKE_SIGNAL]       	= "�������." "\0\0\0\0\0\0\0\0",


	[TXT_BLUE] = "�����   ",
	[TXT_GREEN] = "������� ",
	[TXT_CYAN] = "������� ",
	[TXT_RED] = "������� ",
	[TXT_MAGANTA] = "������� ",
	[TXT_YELLOW] = "������  ",
	[TXT_ORANGE] = "�����.  ",
	[TXT_WHITE] = "�����   ",
	[TXT_BLACK] = "������  ",

	[TXT_IS_NONE] = "  ����. ",
	[TXT_IS_LED] = "��.     ",
	[TXT_IS_SOUND] = "   ��.  ",
	[TXT_IS_SOUND_LED] = "��.��.  ",
	[TXT_IS_VIBRO] = "      �.",
	[TXT_IS_VIBRO_LED] = "��.   �.",
	[TXT_IS_VIBRO_SOUND] = "   ��.�.",
	[TXT_IS_ALL] = "��.��.�.",
};


