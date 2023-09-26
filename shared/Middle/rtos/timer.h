/*******************************************************************************
 *
 * Copyright (c) 2018
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: HoangNH
 *
 * Last Changed By:  $Author: HoangNH $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date: 6/20/18 $
 *
 ******************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MAX_TIMER                   16u
#define TIMER_REPEAT_ONE_TIME       0u
#define TIMER_REPEAT_FOREVER        0xFFu
#define NO_TIMER                    0xFFu

typedef uint8_t SSwTimer;
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
 * @func   TimerInit
 * @brief  None
 * @param  None
 * @retval None
 */
void
TimerInit(void);

/**
 * @func   TimerStart
 * @brief  None
 * @param  None
 * @retval None
 */

uint8_t                                /* OUT: index of timer */
TimerStart(
    char* name,
    uint32_t dwMilSecTick,             /* IN: timer tick */
	uint8_t byRepeats,                 /* IN: number of repeater */
	void (*callback)(void *),            /* IN: callback function*/
    void *pcallbackData                /* IN: parameters */
);

/**
 * @func   TimerChangePeriod
 * @brief  Change period of timer
 * @param  byTimerId
 * @param  dwMilSecTick
 * @retval None
 */
void
TimerChangePeriod(
	uint8_t byTimerId,
	uint32_t dwPeriodTicks
);
    
/**
 * @func   TimerRestart
 * @brief  None
 * @param  None
 * @retval None
 */
uint8_t
TimerRestart(
	uint8_t byTimerId,
	uint32_t dwMilSecTick,
	uint8_t byRepeats
);
    
/**
 * @func   TimerCancel
 * @brief  None
 * @param  None
 * @retval None
 */
uint8_t
TimerStop(
    uint8_t byTimerId
);

/**
 * @func   GetMilSecTick
 * @brief  None
 * @param  None
 * @retval None
 */
uint32_t
GetMilSecTick(void);

/**
 * @func   processTimer
 * @brief  None
 * @param  None
 * @retval None
 */
void
processTimerScheduler(void);

#endif

/* END FILE */
