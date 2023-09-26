/*******************************************************************************
 *
 * Copyright (c) 2020
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: HoangNH
 *
 * Last Changed By:  $Author: HoangNH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date:  10/07/20 11:30 $
 *
 ******************************************************************************/
#ifndef _BUTTON_H_
#define _BUTTON_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/*! @brief Pins input control direction */
#define BUTTON_BOARD_PIN                 { GPIOC, GPIO_Pin_13, 0 }
#define BUTTON_KIT0_PIN                  { GPIOB, GPIO_Pin_5,  0 }
#define BUTTON_KIT1_PIN                  { GPIOB, GPIO_Pin_3,  0 }
#define BUTTON_KIT2_PIN                  { GPIOA, GPIO_Pin_4,  0 }
#define BUTTON_KIT3_PIN               	 { GPIOB, GPIO_Pin_0,  0 }
#define BUTTON_KIT4_PIN               	 { GPIOB, GPIO_Pin_4,  0 }

#define KEY_COUNT_IS_PRESS               10u
#define KEY_TIME_SCAN                    5u // 5ms

#define BUTTON_BOARD_ID                  0u
#define BUTTON_KIT_ID1                   1u
#define BUTTON_KIT_ID2                   2u
#define BUTTON_KIT_ID3                   3u
#define BUTTON_KIT_ID4                   4u
#define BUTTON_KIT_ID5                   5u
#define BUTTON_MAX                 		 6u

#define KEY_TIMEOUT_RELEASE              50u
#define KEY_TIMEOUT_BW2PRESS             400u
#define KEY_TIME_IS_HOLD                 600u
#define KEY_TIME_HOLD1S                  1000u
#define KEY_TIME_HOLD3S                  3000u
#define KEY_TIME_HOLD5S                  5000u
#define KEY_TIME_HOLD10S                 10000u

#define TIMECNT_IS_HOLD                  (KEY_TIME_IS_HOLD / KEY_TIME_SCAN)
#define TIMECNT_IS_RELEASE               (KEY_TIMEOUT_RELEASE / KEY_TIME_SCAN)
#define TIMECNT_BW2PRESS                 (KEY_TIMEOUT_BW2PRESS / KEY_TIME_SCAN)
#define TIMECNT_HOLD1S                   (KEY_TIME_HOLD1S / KEY_TIME_SCAN)
#define TIMECNT_HOLD3S                   (KEY_TIME_HOLD3S / KEY_TIME_SCAN)
#define TIMECNT_HOLD5S                   (KEY_TIME_HOLD5S / KEY_TIME_SCAN)
#define TIMECNT_HOLD10S                  (KEY_TIME_HOLD10S / KEY_TIME_SCAN)

#define BUTTON_EDGE_RISING               0
#define BUTTON_EDGE_FALLING              1

typedef enum {
    BUTTON_EVENT_EDGE,
	BUTTON_EVENT_PRESS,
    BUTTON_EVENT_HOLD,
    BUTTON_EVENT_RELEASE,
    BUTTON_EVENT_MAXIMUM
} button_event_t;

typedef enum {
    BUTTON_TYPE_LOGIC = 0x01,
    BUTTON_TYPE_EDGE  = 0x02
} key_type_t;

typedef void (* button_event_callback)(uint8_t id, uint16_t time);
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   Button_Init
 * @brief  Initialize module button
 * @param  None
 * @retval None
 */
void
Button_Init(void);

/** 
 * @func   Button_RegisterEventCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
Button_RegisterEventCallback(
    button_event_t buttonEvent,
	button_event_callback procbuttonEvent
);

/**
 * @func   ButtonSetMode
 * @brief  Set mode key
 * @param  None
 * @retval None
 */
void
Button_SetMode(
    uint8_t id,
    uint8_t mode
);

/**
 * @func   ButtonGetMode
 * @brief  Get mode key
 * @param  None
 * @retval None
 */
uint8_t
Button_GetMode(
    uint8_t id
);

/**
 * @func   Button_GetLogicInputPin
 * @brief  Button get logic on input pin
 * @param  id: id of input pin
 * @retval logic pin
 */
uint8_t
Button_GetLogicInputPin(
	uint8_t id
);

#endif /* END FILE */
