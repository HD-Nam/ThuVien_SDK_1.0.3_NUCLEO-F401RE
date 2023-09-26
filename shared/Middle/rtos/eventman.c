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
 * Revision:         $Revision: 1.1  $
 * Last Changed:     $Date: 10/07/20 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "eventman.h"
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef EVENT_MANAGER_DEBUG
#define DBG_EVENTMAN_SEND_STR(x)         Debug_SendStr(x)
#define DBG_EVENTMAN_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_EVENTMAN_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_EVENTMAN_SEND_STR(x)
#define DBG_EVENTMAN_SEND_NUM(x)
#define DBG_EVENTMAN_SEND_HEX(x)
#endif

#define SIZE_EVENT_QUEUE              256

#if (SIZE_EVENT_QUEUE & (SIZE_EVENT_QUEUE - 1)) != 0
#error "SIZE_EVENT_QUEUE must be a power of two"
#endif
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static app_state_callback pAppStateFunc = NULL;
static uint8_t pBuffEvent[SIZE_EVENT_QUEUE];
static buffqueue_t eventQueue;
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
 * @func   EventSchedulerInit 
 * @brief  Initializes event scheduler
 * @param  eventQueue
 * @param  byItemSize
 * @param  func
 * @retval None
 */
void
EventSchedulerInit(
    app_state_callback func
) {
    if (func != NULL) {
        pAppStateFunc = func;
        bufInit(pBuffEvent, &eventQueue, sizeof(uint8_t), SIZE_EVENT_QUEUE);
    }
}

/**
 * @func   EventSchedulerAdd 
 * @brief  Add event to queue
 * @param  eventQueue
 * @param  pvItemToQueue
 * @retval None
 */
type_status_t
EventSchedulerAdd(
    const uint8_t pvItemToQueue
) {
    if (bufEnDat(&eventQueue, (uint8_t *)&pvItemToQueue) == ERR_OK) {
        return SUCCESS1;
    }
    
    return FAIL;
}

/**
 * @func   EventScheduler
 * @brief  Proccess event in queue
 * @param  None
 * @retval None
 */
void
processEventScheduler(void) {
	uint8_t event;
    
    if (pAppStateFunc != NULL) {
        if (bufDeDat(&eventQueue, (uint8_t *)&event) == ERR_OK) {
            pAppStateFunc(event);
        }
    }
}

/* END FILE */
