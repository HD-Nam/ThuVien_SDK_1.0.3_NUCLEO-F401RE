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
#ifndef _EVENT_MAN_H_
#define _EVENT_MAN_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
#include "buff.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum {
    SUCCESS1 = 1,
    FAIL = 0,
    errQUEUE_EMPTY = ERR_BUF_EMPTY,
    errQUEUE_FULL  = ERR_BUF_FULL
} type_status_t;

typedef void (*app_state_callback)(uint8_t);
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
 * @func   EventSchedulerInit 
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSchedulerInit(
    app_state_callback func
);

/**
 * @func   EventSchedulerAdd 
 * @brief  Add event to queue
 * @param  pvItemToQueue
 * @retval None
 */
type_status_t
EventSchedulerAdd(
    const uint8_t pvItemToQueue
);

/**
 * @func   EventScheduler 
 * @brief  Proccess event in queue
 * @param  None
 * @retval None
 */
void
processEventScheduler(void);

#endif /* END FILE */
