#ifndef BOARD_H
#define BOARD_H

#define RTC_DRV
#define ADC
#define TIMER1_HW
#define TIMER4_HW

#define PWM1_HW
#define PWM1_HW_PORT0			(GPIOA)
#define PWM1_HW_PORT1			(NULL) /* GPIOA */
#define PWM1_HW_PORT2			(NULL) /* GPIOA */
#define PWM1_HW_PORT3			(NULL) /* GPIOA */

#define PWM1_HW_PIN0			(8)
#define PWM1_HW_PIN1			(9)
#define PWM1_HW_PIN2			(10)
#define PWM1_HW_PIN3			(11)

#define IC4_HW
#define IC4_HW_PORT0			(GPIOB)
#define IC4_HW_PORT1			(NULL) /* GPIOB */
#define IC4_HW_PORT2			(NULL) /* GPIOB */
#define IC4_HW_PORT3			(NULL) /* GPIOB */

#define IC4_HW_PIN0			    (6)
#define IC4_HW_PIN1			    (7)
#define IC4_HW_PIN2			    (8)
#define IC4_HW_PIN3			    (9)

#define PWM4_HW
#define PWM4_HW_PORT0			(GPIOB)
#define PWM4_HW_PORT1			(NULL) /* GPIOB */
#define PWM4_HW_PORT2			(NULL) /* GPIOB */
#define PWM4_HW_PORT3			(NULL) /* GPIOB */

#define PWM4_HW_PIN0			    (6)
#define PWM4_HW_PIN1			    (7)
#define PWM4_HW_PIN2			    (8)
#define PWM4_HW_PIN3			    (9)

#define SPI1_HW
#define SPI1_HW_SDO_PORT    GPIOB
#define SPI1_HW_SDO_PIN     4
#define SPI1_HW_SDI_PORT    GPIOB
#define SPI1_HW_SDI_PIN     5
#define SPI1_HW_CLK_PORT    GPIOB
#define SPI1_HW_CLK_PIN     3

#define LOGGING

#define BUTTONS
#define ENTER_BTN   (0)
#define ESCAPE_BTN  (1)
#define LEFT_BTN    (2)
#define RIGHT_BTN   (3)

#define BUTTON_COUNT    (4)

#define KEYPAD_DPORT0   GPIOB
#define KEYPAD_PIN0     (12)
#define KEYPAD_DPORT1   GPIOC
#define KEYPAD_PIN1     (5)
#define KEYPAD_DPORT2   GPIOC
#define KEYPAD_PIN2     (4)
#define KEYPAD_DPORT3   GPIOB
#define KEYPAD_PIN3     (13)

#define GRAPHIC
#define GRAPHIC_THREAD
#define BCB0802
#define BCB0802_RS_PORT     GPIOC
#define BCB0802_RS_PIN      0
#define BCB0802_RW_PORT     GPIOC
#define BCB0802_RW_PIN      1
#define BCB0802_E_PORT      GPIOC
#define BCB0802_E_PIN       2
#define BCB0802_POWER_PORT  GPIOC
#define BCB0802_POWER_PIN   3

#define BCB0802_D0_PORT     GPIOA
#define BCB0802_DO_PIN      0
#define BCB0802_D1_PORT     GPIOA
#define BCB0802_D1_PIN      1
#define BCB0802_D2_PORT     GPIOA
#define BCB0802_D2_PIN      2
#define BCB0802_D3_PORT     GPIOA
#define BCB0802_D3_PIN      3
#define BCB0802_D4_PORT     GPIOA
#define BCB0802_D4_PIN      4
#define BCB0802_D5_PORT     GPIOA
#define BCB0802_D5_PIN      5
#define BCB0802_D6_PORT     GPIOA
#define BCB0802_D6_PIN      6
#define BCB0802_D7_PORT     GPIOA
#define BCB0802_D7_PIN      7

#define LED_ADDR
#define LED_ADDR_COUNT      1
#define LED_ADDR_PORT       GPIOC
#define LED_ADDR_PIN        7

#define NET_LEGACY
#define DEV_BEH				(NETB_TRX)
#define NOTIFY_COUNT_MAX    (32)
#define NET_WITHOUT_TYPE_BEH
#define LLINK_FREQ          (433920)

#define CMT2300A
#define CMT2300A_CS_PORT    GPIOB
#define CMT2300A_CS_PIN     7
#define CMT2300A_INT_PORT   GPIOB
#define CMT2300A_INT_PIN    6
#define CMT2300A_POW_PORT	GPIOA
#define CMT2300A_POW_PIN	12

#define CMT2300A_SPI_HW     (&spi1)
#define CMT2300A_GPIO_NUM   (2)
#define CMT2300A_IC      	(&ic4)
#define CMT2300A_PWM      	(&pwm4)

#define SOUND
#define PWM_SOUND
#define PWM_SOUND_HW           (&pwm1)
#define PWM_SOUND_HW_CH        (1)
#define RUSSIAN_MELODIES

#define TRANSISTORVOL
#define TRANSISTORVOL_CNT   (2)
#define TRANSISTORVOL_PORT0     GPIOB
#define TRANSISTORVOL_PIN0      (14)
#define TRANSISTORVOL_PORT1     GPIOB
#define TRANSISTORVOL_PIN1      (15)

#define BOOSTER5V
#define BOOSTER5V_PORT			GPIOC
#define BOOSTER5V_PIN			(13)
#define BOOSTER5V_MASK			(1 << BOOSTER5V_PIN)
#define BOOSTER5V_INIT()		DPORT_DOUT_INIT(BOOSTER5V_PORT, BOOSTER5V_MASK)
#define BOOSTER5V_ON()			DPORT_SET(BOOSTER5V_PORT, BOOSTER5V_MASK)
#define BOOSTER5V_OFF()			DPORT_CLR(BOOSTER5V_PORT, BOOSTER5V_MASK)


#define UPOWER
#define UPOWER_CHIP
#define UPOWER_CHIP_CHRG_PORT   (GPIOB)
#define UPOWER_CHIP_CHRG_PIN    (10)
#define UPOWER_CHIP_STAT_PORT   (GPIOB)
#define UPOWER_CHIP_STAT_PIN    (2)
#define UPOWER_CHIP_ADC_PORT    (GPIOB)
#define UPOWER_CHIP_ADC_PIN     (0)
#define UPOWER_CHIP_ADC_CH      (ADC_Channel_8)

/* ">> 1" because in scheme divider */
#define UPOWER_CHIP_NEED_OFF_ADC    (340 >> 1)
#define UPOWER_CHIP_LOW_ADC         (360 >> 1)
#define UPOWER_CHIP_NORMAL_ADC      (380 >> 1)
#define UPOWER_CHIP_PREFULL_ADC     (400 >> 1)
#define UPOWER_CHIP_FULL_ADC        (420 >> 1)

#define RANDOM
#define random32()			(0xffeeffee)

#define SSIGN
#define SSIGN_L_PORT    (GPIOC)
#define SSIGN_L_PIN     (6)
#define SSIGN_M_PORT    (GPIOB)
#define SSIGN_M_PIN     (1)

#endif//BOARD_H
