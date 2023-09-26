/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: ThangDH
 *
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 28/08/17 15:00 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

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
 * @func   searchArrayInArray
 * @brief  None
 * @param  None
 * @retval offset ArrKey match in ArrData OR NOT_FOUND
 */
uint16_t
searchArrayInArray(
	const uint8_t *pArrKey,
	uint8_t keyLength,
	const uint8_t *pArrData,
	uint16_t dataLength
) {
	uint16_t i,j = 0;
    if (dataLength < keyLength) return NOTFOUND;
    
    for (i = 0; i!= dataLength && j != keyLength; i++) {
         if (pArrKey[j] == pArrData[i]) {
             j++;
         }
         else {
             i -= j;
             j = 0;
         }
        
         if (j == keyLength) {
             /* */
             return (i + 1 - keyLength );   
         }      
    } 

    return NOTFOUND;
}

/**
 * @func   hex2Char
 * @brief  None
 * @param  None
 * @retval 
 */
uint8_t
hex2Char(
	uint8_t byHex
) {
	uint8_t byChar;
    
    if (byHex < 10) byChar = byHex + 0x30;
    else byChar = byHex + 55;
           
    return byChar;
}

/**
 * @func   String2Uint
 * @brief  None
 * @param  None
 * @retval 
 */
uint32_t
String2Uint(
    uint8_t *pChar,
	uint8_t offset,
	uint8_t length
) {
	uint8_t pTemp[8];
	uint8_t i;
	uint8_t j = length - 1;
	uint32_t retval = 0;
	uint32_t temp = 1;

    for (i = 0; i < length; i++) {
        pTemp[j - i] = pChar[i + offset] - '0';
    } 
    
    for (i = 0; i < length; i++) {
        if (i == 0) {
            temp = 1;
            retval = 0;
        }
        else 
            temp = temp * 10;

        if (pTemp[i] < 10) {
            retval += pTemp[i] *temp;
        } 
        else {
            return 0xFFFFFFFF;
        }
    }  

    return retval;
}

/**
 * @func   memsetl
 * @brief  None
 * @param  None
 * @retval None
 */
void
memsetl(
	uint8_t *dst,
	uint8_t value,
	uint16_t size
) {
    while (size--) {
        *dst++ = value;
    }    
}

/**
 * @func   memcpyl
 * @brief  None
 * @param  None
 * @retval None
 */
void
memcpyl(
	uint8_t *dst,
	uint8_t *src,
	uint16_t size
) {
    while (size--) {
        *dst++ = *src++;
    }   
}

/**
 * @func   valInRange 
 * @brief  None
 * @param  None
 * @retval None
 */
uint8_t
valInRange(
	uint32_t val,
	uint32_t rmin,
	uint32_t rmax
) {
    return ((val >= rmin) && (val <= rmax));
}

/* END FILE */
