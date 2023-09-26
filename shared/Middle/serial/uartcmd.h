/*******************************************************************************
 *
 * Copyright (c) 2020
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
 * Last Changed:     $Date: 10/07/20 $
 *
 ******************************************************************************/
#ifndef _UART_COMMAND_H_
#define _UART_COMMAND_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "stdint.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef void (* buttoncmd_handle_event)(uint8_t button_id, uint8_t button_state);
typedef void (* buzzercmd_handle_event)(uint8_t buzzer_state);
typedef void (* lcdcmd_handle_event)(char *text);
typedef void (* ledcmd_handle_event)(uint8_t led_id, \
									 uint8_t led_color, \
									 uint8_t led_num_blink, \
									 uint8_t led_interval, \
									 uint8_t led_last_state);
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
 * @func   EventSerial_Init
 * @brief  None
 * @param  None
 * @retval None
 */
void EventSerial_Init(void);

/**
 * @func   EventSerial_SetEventLedCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSerial_SetEventLedCallback(
	ledcmd_handle_event pSerialEvent
);

/**
 * @func   EventSerial_SetEventButtonCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSerial_SetEventButtonCallback(
	buttoncmd_handle_event pSerialEvent
);

/**
 * @func   EventSerial_SetEventBuzzerCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSerial_SetEventBuzzerCallback(
	buzzercmd_handle_event pSerialEvent
);

/**
 * @func   EventSerial_SetEventLcdCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSerial_SetEventLcdCallback(
	lcdcmd_handle_event pSerialEvent
);

#endif

/* END FILE */
