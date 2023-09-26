/*******************************************************************************
 *
 * Copyright (c) 2019
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: HoangNH
 *
 * Last Changed By:  $Author: HoangNH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 2/12/2019 $
 *
 ******************************************************************************/
#ifndef _LED_H_
#define _LED_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/* Compute the value to be set in ARR register to generate signal frequency at 17.57 KHz */
#define LED_TIMER_PERIOD           (SystemCoreClock / 17570) - 1

#define LED_BOARD_PORT				GPIOA
#define LED_BOARD_PIN             	GPIO_Pin_5

#define LED1_RED_PORT             	GPIOB
#define LED1_RED_PIN             	GPIO_Pin_13
#define LED1_GREEN_PORT             GPIOA
#define LED1_GREEN_PIN              GPIO_Pin_11
#define LED1_BLUE_PORT            	GPIOA
#define LED1_BLUE_PIN            	GPIO_Pin_10

#define LED2_RED_PORT             	GPIOA
#define LED2_RED_PIN             	GPIO_Pin_1
#define LED2_GREEN_PORT             GPIOA
#define LED2_GREEN_PIN              GPIO_Pin_0
#define LED2_BLUE_PORT            	GPIOC
#define LED2_BLUE_PIN            	GPIO_Pin_8

#define NUM_OF_LED                  3u
#define NUM_OF_COLOR                3u

/*! @brief Led identify */
enum LedIdx_enum{
	LED_BOARD_ID = 0,
	LED_KIT_ID0 = 1,
	LED_KIT_ID1 = 2,
	LED_ALL_ID = 3
};

/*! @brief Enum led blink */
typedef enum __led_blink_type__ {
    BLINK_RED = 0u,
    BLINK_BLUE = 1u,
	BLINK_GREEN = 2u,
    BLINK_WHITE = 3u,
    BLINK_COLOR_MAX = 4u
} led_blink_type_t;

/*! @brief Led color */
typedef enum __led_color__ {
    LED_COLOR_RED = 0x00u,
    LED_COLOR_GREEN = 0x01u,
    LED_COLOR_BLUE = 0x02u,
    LED_COLOR_WHITE = 0x03u,
    LED_COLOR_BLACK = 0x04u,
	LED_COLOR_YELLOW = 0x05u
} led_color_t;

/*! @brief Led level */
typedef uint8_t led_level_t;

#define isTypeLED(x)                ((x == LED_COLOR_RED) || (x == LED_COLOR_GREEN) || (x == LED_COLOR_BLUE))

#define BLINK_FOREVER               0xFFu
#define TIME_REPEAT_BLINK           8u  // times
#define PERIOD_BLINK                200 // ms

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   LedInit
 * @brief  Initializes module led
 * @param  None
 * @retval None
 */
void
LedControl_Init(void);

/**
 * @func   LedControl_SetColorIndividual
 * @brief  Set color individual of led
 * @param  led_id
 * @param  led_color
 * @param  led_level
 * @retval None
 */
void
LedControl_SetColorIndividual(
	uint8_t led_id,
	led_color_t led_color,
	uint8_t led_level
);

/**
 * @func   LedControl_SetColorGeneral
 * @brief  Set color general of led
 * @param  led_id: identify of led
 *         led_color: color of led (R-G-B-W)
 *         led_level: level of led (0 - 100%)
 * @retval None
 */
void
LedControl_SetColorGeneral(
    uint8_t led_id,
	led_color_t led_color,
	uint8_t led_level
);

/**
 * @func   LedControl_SetAllColor
 * @brief  Led all set color white
 * @param  led_color: color of led (R-G-B-W)
 *         led_level: level of led (0 - 100%)
 * @retval None
 */
void
LedControl_SetAllColor(
	uint8_t led_color,
	uint8_t led_level
);

/**
 * @func   LedControl_BlinkStart
 * @brief  Blink start led
 * @param  led_id:
 *         led_color:
 *         led_numRepeat:
 *         led_interval:
 *         led_lastState:
 * @retval None
 */
void
LedControl_BlinkStart(
	uint8_t led_id,
	led_blink_type_t led_blink,
	uint8_t led_numRepeat,
	uint16_t led_interval,
	uint8_t led_lastState
);

/**
 * @func   LedControl_SendPacketRespond
 * @brief  Respond frame status of led
 * @param  led_id: identify of led
 *         led_color: color of led (R-G-B-W)
 *         led_level: level of led (0 - 100%)
 */
void
LedControl_SendPacketRespond(
	uint8_t led_id,
	uint8_t led_color,
	uint8_t led_level
);

#endif

/* END FILE */
