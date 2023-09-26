/******************************************************************************
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
 * Last Changed By:  $Author: hoangnh $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 1/23/18 $
 *
 ******************************************************************************/
#ifndef _BUFF_H_
#define _BUFF_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define ERR_OK                         0x00 
#define ERR_BUF_FULL                   0x01
#define ERR_BUF_EMPTY                  0x02     
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/*!
 * FIFO structure
 */
typedef struct __buff_queue__ {
    
    uint16_t wSize;      /*< Size of buffer */
    
    uint16_t wCountEle;  /*< Count element */
    
    uint8_t byItemSize; /*< The size of each items that the queue will hold. */
    
    uint16_t wHeadIndex; /*< Points to the beginning of the queue storage area. */
    
    uint16_t wTailIndex; /*< Points to the uint8_t at the end of the queue storage area */
    
    uint8_t *pData;    /*< Data memory */
    
} buffqueue_t, *buffqueue_p;
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
 * @func   bufInit
 * @brief  Initializes the FIFO structure
 * @param  pBuffer: Data to be pushed into the FIFO
 * @param  pQueue: Pointer to the FIFO object
 * @param  sizeofElement: Size of a element in the buffer
 * @param  numberOfElement: Size of the buffer
 * @retval None
 */
void
bufInit(
    void *pBuffer,
    buffqueue_p pQueue,
    uint8_t sizeofElement,
    uint16_t numberOfElement
);

/**
 * @func   bufNumEle
 * @brief  Determine number of uint8_ts in FIFO has not been processed
 * @param  pQueue: Pointer to the FIFO object
 * @retval Number of uint8_ts in FIFO
 */
uint16_t
bufNumItems(
    buffqueue_p pQueue
);

/**
 * @func   bufIsFull
 * @brief  Checks if the FIFO is full
 * @param  pQueue: Pointer to the FIFO object
 * @retval TRUE of FALSE
 */
uint8_t
bufIsFull(
    buffqueue_p pQueue
);

/**
 * @func   bufIsEmpty
 * @brief  Checks if the FIFO is empty
 * @param  pQueue: Pointer to the FIFO object
 * @retval TRUE of FALSE
 */
uint8_t
bufIsEmpty(
    buffqueue_p pQueue
);

/**
 * @func   bufFlush
 * @brief  Flushes the FIFO
 * @param  pQueue: Pointer to the FIFO object
 * @retval None
 */
void
bufFlush(
    buffqueue_p pQueue
);

/**
 * @func   bufEnDat
 * @brief  Pushes data to the FIFO
 * @param  pQueue: Pointer to the FIFO object
 * @param  pReceiverData: Received data to be pushed into the FIFO
 * @retval ERR_OK or ERR_BUF_FUL
 */
uint8_t
bufEnDat(
    buffqueue_p pQueue,
    uint8_t *pReceiverData
);

/**
 * @func   bufDeDat
 * @brief  Pops data from the FIFO
 * @param  pQueue: Pointer to the FIFO object
 * @param  pBuffer: Data in the FIFO popped into the buffer
 * @retval ERR_OK or ERR_BUF_EMPTY
 */
uint8_t
bufDeDat(
    buffqueue_p pQueue,
    uint8_t *pBuffer
);

#endif /* END FILE */
