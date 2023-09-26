/*******************************************************************************
 *
 * Copyright (c) 2020
 * Lumi, JSC.
 * All Rights Resered
 *
 *
 * Description:
 *
 * Author: HoangNH
 *
 * Last Changed By:  $Author: HoangNH $
 * Revision:         $Revision:  $
 * Last Changed:     $Date: 10/7/2020 $
 *
 ******************************************************************************/
#ifndef _BUZZER_H_
#define _BUZZER_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define BUZZER_PORT				    GPIOC
#define BUZZER_PIN             	    GPIO_Pin_9

#define BUZZ_PWM                    { BUZZER_PORT, BUZZER_PIN }
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typedef struct _tone_buzz_ {
    uint16_t freq;
    uint16_t duration;
} tone_t, *tone_p;
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
 * @func   BuzzerControl_Init
 * @brief  Initialize module buzzer
 * @param  None
 * @retval None
 */
void
BuzzerControl_Init(void);

/**
 * @func   BuzzerControl_SetMelody
 * @brief  Buzzer set melody
 * @param  pListTone
 * @retval None
 */
void 
BuzzerControl_SetMelody(
    tone_p pListTone
);

/**
 * @func   BuzzerControl_SendPacketRespond
 * @brief  Respond frame status of buzzer
 * @param  buzzer_state: 0 - 100 %
 * @retval None
 */
void
BuzzerControl_SendPacketRespond(
	uint8_t buzzer_state
);

/**
 * @func   BuzzerControl_SetDutyCycle
 * @brief  Set duty cycle PWM
 * @param  dutycycle
 * @retval None
 */
void
BuzzerControl_SetDutyCycle(
    uint8_t dutycycle
);

/**
 * @func   BUZZTIM_SetPreriod
 * @brief  Set period PWM
 * @param  period
 * @retval None
 */
void
BuzzerControl_SetPreriod(
	uint32_t period
);

#endif /* END FILE */
