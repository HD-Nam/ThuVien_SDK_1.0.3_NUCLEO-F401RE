/*******************************************************************************
 *
 * Copyright (c) 2018
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
 * Last Changed:     $Date: 9/15/18 16:30 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "../rtos/timer.h"
#include "stm32f401re_rcc.h"
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef TIMER_SYSTICK_DEBUG
#define DBG_TIMER_SEND_STR(x)         Debug_SendStr(x)
#define DBG_TIMER_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_TIMER_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_TIMER_SEND_STR(x)
#define DBG_TIMER_SEND_NUM(x)
#define DBG_TIMER_SEND_HEX(x)
#endif

typedef struct _TIMER_ {
    char *name;
    uint32_t milSecStart;
    uint32_t milSecTimeout;
    uint8_t repeats;
    void (*callbackFunc)(void *);
    void *pCallbackData;
} TIMER_t, *TIMER_p;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static volatile TIMER_t g_pTimerHandle[MAX_TIMER] = { NULL };
static volatile uint32_t g_wMilSecTickTimer = 0;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static uint8_t TimeExpired(uint8_t byTimerId);
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
TimerInit(void) {
    RCC_ClocksTypeDef RCC_Clocks;

    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.SYSCLK_Frequency/1000);
    NVIC_SetPriority(SysTick_IRQn, 1);
    memsetl((uint8_t *)g_pTimerHandle, 0, sizeof(g_pTimerHandle));
    DBG_TIMER_SEND_STR("$ TimerInit\n");
}

/**
 * @func   TimerStart
 * @brief  None
 * @param  name,
 * @param  wMilSecTick: timer tick
 * @param  byRepeats: number of repeater
 * @param  callback: callback function
 * @param  pcallbackData : parameters
 * @retval Index of timer
 */
uint8_t
TimerStart(
    char* name,
	uint32_t wMilSecTick,
	uint8_t byRepeats,
	void (*callback)(void *),
    void *pcallbackData
) {   
    for (uint8_t i = 0; i < MAX_TIMER; i++)
    {
        if (g_pTimerHandle[i].callbackFunc == NULL)
        {
            g_pTimerHandle[i].name = name;
            g_pTimerHandle[i].callbackFunc = callback;
            g_pTimerHandle[i].repeats = byRepeats;
            g_pTimerHandle[i].pCallbackData = pcallbackData;
            g_pTimerHandle[i].milSecStart = GetMilSecTick();
            g_pTimerHandle[i].milSecTimeout = wMilSecTick;
            
            DBG_TIMER_SEND_STR("$ TimerStart. ");
            DBG_TIMER_SEND_STR(g_pTimerHandle[i].name);
            DBG_TIMER_SEND_STR("Sec start ");
            DBG_TIMER_SEND_NUM(g_pTimerHandle[i].milSecStart);
            DBG_TIMER_SEND_STR(" Id = "); 
            DBG_TIMER_SEND_NUM(i);
            DBG_TIMER_SEND_STR(", repeat = ");
            DBG_TIMER_SEND_NUM(g_pTimerHandle[i].repeats);
            DBG_TIMER_SEND_STR(", timeCall = ");
            DBG_TIMER_SEND_NUM(wMilSecTick);
            DBG_TIMER_SEND_STR("\n");

            return i;
        }
    }
    
    return NO_TIMER;
}

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
    uint32_t dwMilSecTick
) {
    if (byTimerId == NO_TIMER) return; 
    g_pTimerHandle[byTimerId].milSecTimeout = dwMilSecTick;
}


/**
 * @func   TimerRestart
 * @brief  None
 * @param  None
 * @retval None
 */
uint8_t
TimerRestart(
    uint8_t byTimerId,
    uint32_t wMilSecTick,
    uint8_t byRepeats
) {   
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL))
        return 0;
    
    DBG_TIMER_SEND_STR("$ TimerRestart. ");
    DBG_TIMER_SEND_STR(g_pTimerHandle[byTimerId].name);
    DBG_TIMER_SEND_STR(" Id = "); 
    DBG_TIMER_SEND_NUM(byTimerId);
    DBG_TIMER_SEND_STR(", repeat = ");
    DBG_TIMER_SEND_NUM(byRepeats);
    DBG_TIMER_SEND_STR("\n");
    
    g_pTimerHandle[byTimerId].repeats = byRepeats;
    g_pTimerHandle[byTimerId].milSecTimeout = wMilSecTick;
    g_pTimerHandle[byTimerId].milSecStart = GetMilSecTick();
    
    return 1;
}

/**
 * @func   TimerCancel
 * @brief  None
 * @param  None
 * @retval None
 */
uint8_t
TimerStop(
    uint8_t byTimerId
) {   
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL))
        return 0;
    
    DBG_TIMER_SEND_STR("$ TimerCancel. ");
    DBG_TIMER_SEND_NUM(g_wMilSecTickTimer);
    DBG_TIMER_SEND_STR(" ");
    DBG_TIMER_SEND_STR(g_pTimerHandle[byTimerId].name);
    DBG_TIMER_SEND_STR(" Id = "); 
    DBG_TIMER_SEND_NUM(byTimerId);
    DBG_TIMER_SEND_STR("\n");
    
    g_pTimerHandle[byTimerId].name = NULL;
    g_pTimerHandle[byTimerId].callbackFunc = NULL;
    g_pTimerHandle[byTimerId].repeats = 0;
    g_pTimerHandle[byTimerId].milSecTimeout = 0;
    g_pTimerHandle[byTimerId].milSecStart = 0;
    
    return 1;
}


/**
 * @func   GetMilSecTick
 * @brief  None
 * @param  None
 * @retval None
 */
uint32_t
GetMilSecTick(void) {
    return g_wMilSecTickTimer;
}

/**
 * @func   processTimer
 * @brief  None
 * @param  None
 * @retval None
 */
void
processTimerScheduler(void) {
    void (*callbackfunc)(void *);
    void *pPrameter;
    
    for (uint8_t i = 0; i < MAX_TIMER; i++)
    {
        if ((g_pTimerHandle[i].callbackFunc != NULL) && TimeExpired(i))
        {
            callbackfunc = g_pTimerHandle[i].callbackFunc;
            pPrameter = g_pTimerHandle[i].pCallbackData;
            
            DBG_TIMER_SEND_STR("$ TimerProcess. ");
            DBG_TIMER_SEND_NUM(g_wMilSecTickTimer);
            DBG_TIMER_SEND_STR(" ");
            DBG_TIMER_SEND_STR(g_pTimerHandle[i].name);
            DBG_TIMER_SEND_STR(", ID = ");
            DBG_TIMER_SEND_NUM(i);
            DBG_TIMER_SEND_STR(", repeat = ");
            DBG_TIMER_SEND_NUM(g_pTimerHandle[i].repeats);
            DBG_TIMER_SEND_STR(", CB = ");
            DBG_TIMER_SEND_NUM((uint32_t)callbackfunc);
            DBG_TIMER_SEND_STR("\n");
            
            if ((g_pTimerHandle[i].repeats != TIMER_REPEAT_FOREVER) && \
                (g_pTimerHandle[i].repeats != TIMER_REPEAT_ONE_TIME))
                 g_pTimerHandle[i].repeats--;
            
            if (g_pTimerHandle[i].repeats == TIMER_REPEAT_ONE_TIME) {
                 DBG_TIMER_SEND_STR("$ CANCEL \n");
                 TimerStop(i);
            }
            
            callbackfunc(pPrameter);
        }
    }
}

/**
 * @func   TimeExpired
 * @brief  Check timer has expired
 * @param  byTimerId: Timer identifier
 * @retval TRUE or FALSE
 */
uint8_t
TimeExpired(
    uint8_t byTimerId
) {
    uint32_t wMilSecTick;
    uint32_t wDeltaMilSec = 0;
    
    __disable_irq();
    wMilSecTick = GetMilSecTick();
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL))
    {
    	__enable_irq();
        return 0;
    }
      
    if (wMilSecTick >= g_pTimerHandle[byTimerId].milSecStart) 
    {
        wDeltaMilSec = wMilSecTick - g_pTimerHandle[byTimerId].milSecStart;
    } 
    else
    {
        wDeltaMilSec = g_pTimerHandle[byTimerId].milSecStart - wMilSecTick;
        wDeltaMilSec = 0xFFFFFFFFU - wDeltaMilSec;
    }
    
    if (wDeltaMilSec < g_pTimerHandle[byTimerId].milSecTimeout)
    {
    	__enable_irq();
        return 0;
    }
    
    g_pTimerHandle[byTimerId].milSecStart = wMilSecTick;
    
    __enable_irq();
    
    return 1;
}

/**
 * @func   SysTick_Handler
 * @brief  None
 * @param  None
 * @retval None
 */
void
SysTick_Handler(void) {
    g_wMilSecTickTimer++;
}

/* END FILE */
