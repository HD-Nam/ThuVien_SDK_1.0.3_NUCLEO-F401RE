/******************************************************************************
 *				 _ _                                             _ _
				|   |                                           (_ _)
				|   |        _ _     _ _   _ _ _ _ _ _ _ _ _ _   _ _
				|   |       |   |   |   | |    _ _     _ _    | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |_ _ _  |   |_ _|   | |   |   |   |   |   | |   |
				|_ _ _ _ _| |_ _ _ _ _ _| |_ _|   |_ _|   |_ _| |_ _|
								(C)2021 Lumi
 * Copyright (c) 2021
 * Lumi, JSC.
 * All Rights Reserved
 *
 * flash.h
 *
 * Description:
 *
 *  Created on: Oct 22, 2021
 *      Author: PhuongNP
 * Last Changed By:  $Author: 		$
 * Revision:         $Revision:	V1.0.0$
 * Last Changed:     $Date: 		$
 *
 ******************************************************************************/
#ifndef MIDDLE_FLASH_FLASH_H_
#define MIDDLE_FLASH_FLASH_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "stm32f4xx_flash.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define USER_FLASH_FIRST_PAGE_ADDRESS 			0x08010000		//Sector 4(64kB)
#define USER_FLASH_PAGE_SIZE					(128*1024)		//128kB

#define FLASH_USERDATA_VALID							0x5A5A5A5A
#define FLASH_USERDATA_INVALID							0x50505050
#define FLASH_USERDATA_IDLE								0xFFFFFFFF
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/*
typedef struct{
	uint8_t NumberID;
	u16_t Revolution;
	u16_t Cycle;
	u8_t Direction;
	u8_t HoldState;
}Motor_t, *p_Motor_t;
*/

typedef struct{
	uint32_t Used;
	uint8_t Font;
}UserData_t, *p_UserData_t;

#define WORD_SIZE					(sizeof(uint32_t))
#define USERDATA_BYTES_SIZE			(sizeof(UserData_t))
#define USERDATA_WORDS_SIZE			(sizeof(UserData_t)/WORD_SIZE)
#define MAX_NUMBER_OF_USERDATA_IN_SECTOR	((uint32_t)(USER_FLASH_PAGE_SIZE/USERDATA_WORDS_SIZE))
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void FLASH_Init(void);
uint8_t FLASH_IsUserDataChange(void);
void FLASH_RamToFlash(void);
UserData_t *FLASH_GetUserData();

/******************************************************************************/




#endif /* MIDDLE_FLASH_FLASH_H_ */
