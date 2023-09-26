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
 * Last Changed:     $Date: 10/7/2020 $
 *
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "buzzer.h"
#include "serial.h"
#include "stm32f401re_rcc.h"
#include "stm32f401re_gpio.h"
#include "stm32f401re_tim.h"
#include "timer.h"
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct _pwm_pin_ {
    GPIO_TypeDef * port;
    uint16_t pin;
} pwmPin;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static uint8_t bBuzzInit = 0;
static uint8_t idBuzz = NO_TIMER;
static const pwmPin pinBuzz = BUZZ_PWM;
static tone_p gpToneList;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void BuzzPlay(uint16_t freq, uint16_t duration);
static void BuzzOff(void *arg);
static void BUZZTIM_InitTimer(uint8_t timidx, uint32_t prescale, uint32_t period);
static void BUZZTIM_InitPWMChannel(pwmPin pin);
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
BuzzerControl_Init(void) {
	BUZZTIM_InitTimer(0, 32, 5000);
	BUZZTIM_InitPWMChannel(pinBuzz);
    bBuzzInit = 1;
}

/**
 * @func   BuzzOff
 * @brief  None
 * @param  None
 * @retval None
 */
static void
BuzzOff(
    void *arg
) {
    idBuzz = NO_TIMER;
    gpToneList++;  /* Next Element */
    if ((gpToneList->freq == 0) && (gpToneList->duration == 0)) {
    	BuzzerControl_SetDutyCycle(0);
    }
    else {
        BuzzPlay(gpToneList->freq, gpToneList->duration);
    }
}

/**
 * @func   BuzzOff
 * @brief  None
 * @param  None
 * @retval None
 */
static void
BuzzPlay(
    uint16_t freq,
	uint16_t duration
) {
    if (freq == 0) {
    	BuzzerControl_SetDutyCycle(0);
    }
    else if (freq == 0xFFFFU) {
    	BuzzerControl_SetPreriod(5000);
    	BuzzerControl_SetDutyCycle(101);
    }
    else {
        uint32_t period = 750000 / freq;
        BuzzerControl_SetPreriod(period);
        BuzzerControl_SetDutyCycle(5);
    }

    if (idBuzz != NO_TIMER) {
        TimerStop(idBuzz);
        idBuzz = NO_TIMER;
    }

    idBuzz = TimerStart("buzz", duration, TIMER_REPEAT_ONE_TIME, BuzzOff, NULL);
}

/**
 * @func   BuzzerControl_SetMelody
 * @brief  Buzzer set melody
 * @param  pListTone
 * @retval None
 */
void 
BuzzerControl_SetMelody(
    tone_p pListTone   
) {
    if (!bBuzzInit) return;
    gpToneList = pListTone;
    BuzzPlay(gpToneList->freq, gpToneList->duration); 
}

/**
 * @func   BuzzerControl_SendPacketRespond
 * @brief  Respond frame status of buzzer
 * @param  buzzer_state: times beep
 * @retval None
 */
void
BuzzerControl_SendPacketRespond(
	uint8_t buzzer_state
) {
	uint8_t byPayload[CMD_SIZE_OF_PAYLOAD_BUZZER];

	byPayload[0] = buzzer_state;

	/* Send message uart button press 1 time */
	Serial_SendPacket(CMD_OPT_NOT_USE,
					  CMD_ID_BUZZER,
					  CMD_TYPE_RES,
					  byPayload,
					  sizeof(byPayload));
}

/**
 * @func   TIM_Init
 * @brief  None
 * @param  None
 * @retval None
 */
static void
BUZZTIM_InitTimer(
    uint8_t timidx,
	uint32_t prescale,
	uint32_t period
) {
    TIM_TimeBaseInitTypeDef TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* NOT For TIM 1 & TIM8 */
    TimeBaseStructure.TIM_Prescaler = prescale - 1;
    TimeBaseStructure.TIM_Period = period - 1;
    TimeBaseStructure.TIM_ClockDivision = 0;
    TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TimeBaseStructure);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @func   TIMPWM_InitChannel
 * @brief  None
 * @param  None
 * @retval None
 */
static void
BUZZTIM_InitPWMChannel(
    pwmPin pin
) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    /* GPIO Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* Configure pin in output push-pull mode */
    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    GPIO_PinAFConfig(BUZZER_PORT, GPIO_PinSource9, GPIO_AF_TIM3);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* PWM1 Mode */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

/**
 * @func   BuzzerControl_SetDutyCycle
 * @brief  Set duty cycle PWM
 * @param  dutycycle
 * @retval None
 */
void
BuzzerControl_SetDutyCycle(
    uint8_t dutycycle
) {
	uint32_t val;
    uint32_t maxval;

    maxval = TIM3->ARR;
    val = dutycycle * maxval / 100;

	TIM_SetCompare4(TIM3, val);
}

/**
 * @func   BUZZTIM_SetPreriod
 * @brief  Set period PWM
 * @param  period
 * @retval None
 */
void
BuzzerControl_SetPreriod(
	uint32_t period
) {
    TIM3->ARR = period - 1;
}
