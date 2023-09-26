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
 * Last Changed:     $Date: 10/7/2020 $
 *
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "led.h"
#include "timer.h"
#include "serial.h"
#include "stm32f401re_rcc.h"
#include "stm32f401re_gpio.h"
#include "stm32f401re_tim.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static cmd_led_indicator_t ledBlink;
static SSwTimer blinkTimer = NO_TIMER;
static led_level_t g_brightnessLed = 0;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void BlinkStop(cmd_led_indicator_t *ledinfor);
static void LEDGPIO_Config(void);
static void LEDTIM_Config(void);
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
LedControl_Init(void) {
	// Initialize pins GPIO led
	LEDGPIO_Config();

	// Initialize TIMER led
	LEDTIM_Config();
}

/**
 * @func   LedControl_SetColorIndividual
 * @brief  Set color individual of led
 * @param  led_id: identify of led
 *         led_color: color of led (R-G-B-W)
 *         led_level: level of led (0 - 100%)
 * @retval None
 */
void
LedControl_SetColorIndividual(
	uint8_t led_id,
	led_color_t led_color,
	uint8_t led_level
) {
	uint32_t channelPulse;

    if ((led_id >= NUM_OF_LED) || !isTypeLED(led_color) || (led_level > 100))
        return;
    
    // Get brightness current
    if (led_level == 0)
    {
    	g_brightnessLed = 100;
    }
    else
    {
    	g_brightnessLed = led_level;
    }

    // Convert value level to value pwm counter
    channelPulse = (((uint32_t) led_level * (LED_TIMER_PERIOD - 1)) / 100);

    if (led_id == LED_BOARD_ID)
    {
    	if (led_level != 0)
    	{
    		GPIO_SetBits(LED_BOARD_PORT, LED_BOARD_PIN);
    	}
    	else
    	{
    		GPIO_ResetBits(LED_BOARD_PORT, LED_BOARD_PIN);
    	}
    }
    else if (led_id == LED_KIT_ID0)
	{
    	if (led_color == LED_COLOR_RED)
    	{
    		TIM_SetCompare1(TIM1, channelPulse);
    	}
    	else if (led_color == LED_COLOR_GREEN)
    	{
    		TIM_SetCompare4(TIM1, channelPulse);
    	}
    	else /* (led_color == LED_COLOR_BLUE) */
    	{
			TIM_SetCompare3(TIM1, channelPulse);
		}
	}
    else /* (led_id == LED_KIT_ID1) */
    {
    	if (led_color == LED_COLOR_RED)
    	{
    		TIM_SetCompare2(TIM2, channelPulse);
    	}
    	else if (led_color == LED_COLOR_GREEN)
    	{
    		TIM_SetCompare1(TIM2, channelPulse);
    	}
    	else /* (led_color == LED_COLOR_BLUE) */
    	{
		}
    }
}

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
) {
    switch (led_color) {
        case LED_COLOR_RED:
        	LedControl_SetColorIndividual(led_id, LED_COLOR_RED, led_level);
        	LedControl_SetColorIndividual(led_id, LED_COLOR_BLUE, 0);
        	LedControl_SetColorIndividual(led_id, LED_COLOR_GREEN, 0);
            break;

        case LED_COLOR_GREEN:
        	LedControl_SetColorIndividual(led_id, LED_COLOR_GREEN, led_level);
        	LedControl_SetColorIndividual(led_id, LED_COLOR_RED, 0);
        	LedControl_SetColorIndividual(led_id, LED_COLOR_BLUE, 0);
			break;

        case LED_COLOR_BLUE:
        	LedControl_SetColorIndividual(led_id, LED_COLOR_BLUE, led_level);
        	LedControl_SetColorIndividual(led_id, LED_COLOR_RED, 0);
        	LedControl_SetColorIndividual(led_id, LED_COLOR_GREEN, 0);
            break;

        case LED_COLOR_WHITE:
        	LedControl_SetColorIndividual(led_id, LED_COLOR_BLUE, led_level);
        	LedControl_SetColorIndividual(led_id, LED_COLOR_RED, led_level);
        	LedControl_SetColorIndividual(led_id, LED_COLOR_GREEN, led_level);
        	break;

        case LED_COLOR_YELLOW:
			LedControl_SetColorIndividual(led_id, LED_COLOR_BLUE, 0);
			LedControl_SetColorIndividual(led_id, LED_COLOR_RED, led_level);
			LedControl_SetColorIndividual(led_id, LED_COLOR_GREEN, led_level);
			break;

        case LED_COLOR_BLACK:
			LedControl_SetColorIndividual(led_id, LED_COLOR_BLUE, 0);
			LedControl_SetColorIndividual(led_id, LED_COLOR_RED, 0);
			LedControl_SetColorIndividual(led_id, LED_COLOR_GREEN, 0);
			break;

        default:
            break;
    }
}

/**
 * @func   LedGetLevelAllLed
 * @brief  Get color of all led
 * @param  None
 * @retval level
 */
static void LedGetAllLastState(void) {

}

/**
 * @func   LedToggle
 * @brief  Toggle led
 * @param  led_id: identify of led
 *         led_color: color of led
 * @retval None
 */
static void LedToggle(uint8_t led_id, led_color_t led_color)
{
    uint8_t led_level;
    static uint8_t bToggle = 0, bToggleAll = 0;

    if (led_id > LED_ALL_ID) return;

    if (led_id == LED_ALL_ID) {
        if (bToggleAll == 0)
        {
        	led_level = g_brightnessLed;
        }
        else /* (bToggleAll == 1) */
        {
        	led_level = 0;
        }
        bToggleAll = !bToggleAll;
        LedControl_SetAllColor(led_color, led_level);
    }
    else {
        if (bToggle == 0)
        {
        	led_level = g_brightnessLed;
        }
        else /* (bToggle == 1) */
        {
        	led_level = 0;
        }
        bToggle = !bToggle;
        LedControl_SetColorGeneral(led_id, led_color, led_level);
    }
}

/**
 * @func   pFuncBlink
 * @brief  Blink led
 * @param  arg
 * @retval None
 */
static void pFuncBlink(void *arg)
{
    cmd_led_indicator_t *pLedinfor = (cmd_led_indicator_t *)arg;

    if (pLedinfor->counter == 0) {
        BlinkStop(pLedinfor);
        return;
    }
    if ((pLedinfor->counter != BLINK_FOREVER) && (pLedinfor->counter != 0)) {
    	pLedinfor->counter--;
    }

    switch ((led_blink_type_t)pLedinfor->color)
    {
        case BLINK_RED:
        	LedToggle(pLedinfor->numID, LED_COLOR_RED);
            break;

        case BLINK_GREEN:
        	LedToggle(pLedinfor->numID, LED_COLOR_GREEN);
            break;

        case BLINK_BLUE:
			LedToggle(pLedinfor->numID, LED_COLOR_BLUE);
			break;

        case BLINK_WHITE:
			LedToggle(pLedinfor->numID, LED_COLOR_WHITE);
			break;

        default:
            return;
    }

    /* Change period of timer */
    TimerChangePeriod(blinkTimer, pLedinfor->interval);
}

/**
 * @func   BlinkStop
 * @brief  Blink stop
 * @param  pLedinfor:
 * @retval None
 */
static void BlinkStop(cmd_led_indicator_t *pLedinfor)
{
    if (blinkTimer != NO_TIMER)
    {
        TimerStop(blinkTimer);
        blinkTimer = NO_TIMER;
    }

    if (pLedinfor->numID == LED_ALL_ID)
    {
        if (pLedinfor->laststate == 0xFF)
        {
            LedGetAllLastState();
        }
    }
    else {
        if (pLedinfor->laststate == 0xFF)
        {
            LedGetAllLastState();
        }
        else
        {
            LedControl_SetColorGeneral(pLedinfor->numID, (led_color_t)pLedinfor->laststate, g_brightnessLed);
        }
    }

    pLedinfor->counter = 0;
    pLedinfor->color = BLINK_COLOR_MAX;
    pLedinfor->interval = 0;
}

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
) {
	if ((led_id > NUM_OF_LED) || (led_blink >= BLINK_COLOR_MAX))
		return;

	ledBlink.numID = led_id;
	ledBlink.color = (uint8_t)led_blink;
	ledBlink.counter = led_numRepeat;
	ledBlink.interval = led_interval;
	ledBlink.laststate = led_lastState;

	blinkTimer = TimerStart("BlinkLed", 1, TIMER_REPEAT_FOREVER, pFuncBlink, &ledBlink);
}

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
) {
    for (uint8_t i = 0; i < NUM_OF_LED; i++) {
    	LedControl_SetColorGeneral(i, led_color, led_level);
    }
}

/**
 * @func   LedControl_SendPacketRespond
 * @brief  Respond frame status of led
 * @param  led_id: identify of led
 *         led_color: color of led (R-G-B-W)
 *         led_level: level of led (0 - 100%)
 * @retval None
 */
void
LedControl_SendPacketRespond(
	uint8_t led_id,
	uint8_t led_color,
	uint8_t led_level
) {
	uint8_t byPayload[CMD_SIZE_OF_PAYLOAD_RES_LED];

	byPayload[0] = led_id;
	byPayload[1] = led_color;
	byPayload[2] = led_level;

	/* Send command led to MCU */
	Serial_SendPacket(CMD_OPT_NOT_USE,
					  CMD_ID_LED,
					  CMD_TYPE_SET,
					  byPayload,
					  sizeof(byPayload));
}

/**
  * @func   LEDGPIO_Config
  * @brief  Configure GPIO for led
  * @param  None
  * @retval None
  */
static void LEDGPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA, GPIOB Clocks enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

	/* GPIOA Configuration: Channel 0, 1, 3, 10 and 11 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = LED1_GREEN_PIN | LED1_BLUE_PIN | LED2_RED_PIN | LED2_GREEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);

	/* GPIOB Configuration: Channel 13 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = LED1_RED_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_TIM1);

	/* GPIOA Configuration: Board STM32 */
	GPIO_InitStructure.GPIO_Pin = LED_BOARD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(LED_BOARD_PORT, &GPIO_InitStructure);
}

/**
  * @func   LEDTIM_Config
  * @brief  Configure the TIM1 and TIM2 Pins.
  * @param  None
  * @retval None
  */
static void LEDTIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t ChannelPulse = 0;

	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
	ChannelPulse = (uint16_t) (((uint32_t) 0 * (LED_TIMER_PERIOD - 1)) / 100);

	/* TIM1 and TIM2 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = LED_TIMER_PERIOD;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Channel 1, 2, 3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	/* TIM1 and TIM2 counter enable */
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	/* TIM1 and TIM2 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
}

/* END FILE */
