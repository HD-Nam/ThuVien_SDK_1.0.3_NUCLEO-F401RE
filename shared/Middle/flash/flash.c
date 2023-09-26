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
 * flash.c
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
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "flash.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/                     // khoi tao du lieu step

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static UserData_t UserData;
static uint32_t UserDataAddress = USER_FLASH_FIRST_PAGE_ADDRESS;
uint32_t Arr=2;                          // khoi tao gia tri
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static uint8_t UserData_FindAddress( void );
static uint8_t UserData_Write( void );
static void UserData_Read( void );
static void UserData_Erase( void );
void Config_Usart();
uint16_t USART2_IRQhandler(uint16_t data);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/******************************************************************************/


void FLASH_Init(void) {
	/* Next commands may be used in SysClock initialization function
	   In this case using of FLASH_Init is not obligatorily */
	/* Enable Prefetch Buffer */
	FLASH_PrefetchBufferCmd( ENABLE);
	/* Flash 2 wait state */
	FLASH_SetLatency( FLASH_Latency_2);

	if( UserData_FindAddress() == SUCCESS){
//		printf("Found User Data Address 0x%lu!\n", UserDataAddress);
	} else {
//		printf("Erase sector!\n");
		UserData_Erase();
		UserDataAddress = USER_FLASH_FIRST_PAGE_ADDRESS;
	}
}

uint8_t FLASH_IsUserDataChange(void){
	return (!(memcmp((void *)UserDataAddress, (void *)&UserData, USERDATA_BYTES_SIZE) == 0));
}

void FLASH_RamToFlash(void){
//	if( FLASH_IsUserDataChange() ){
//		printf("Run\n");
		UserData_Write();
//	}
}

UserData_t *FLASH_GetUserData(){
	return &UserData;
}
/************************************/
static uint8_t UserData_FindAddress(void){
	for(uint32_t i = 0; i < MAX_NUMBER_OF_USERDATA_IN_SECTOR; i++){
		UserData_Read();	//Read user data to

		if( UserData.Used == FLASH_USERDATA_VALID || UserData.Used == FLASH_USERDATA_IDLE){
			return SUCCESS;
		}
		UserDataAddress += USERDATA_BYTES_SIZE;
	}
	return ERROR;
}

static uint8_t UserData_Clear(void){
	if( FLASH_ProgramWord((uint32_t)UserDataAddress, FLASH_USERDATA_INVALID) == FLASH_COMPLETE){
		if( *(__IO uint32_t*)UserDataAddress != FLASH_USERDATA_INVALID){
			FLASH_Lock();
//			printf("UserData_Clear error\n");
			return ERROR;
		}
	} else {
	  /* Error occurred while writing data in Flash memory.
		 User can add here some code to deal with this error */
		FLASH_Lock();
	//	printf("UserData_Clear error 2\n");
		return ERROR;
	}
	return SUCCESS;
//	UserDataAddress += USERDATA_BYTES_SIZE;
}

uint8_t UserData_Write(void){
	/* Enable the flash control register access */
	FLASH_Unlock();
	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* Clear pending flags (if any) */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
				  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	/*Clear IsUsed in userdata*/
	if( UserData_Clear() != SUCCESS ){
	//	printf("Error clear\n");
		return ERROR;
	}
	UserDataAddress += USERDATA_BYTES_SIZE;

	if( UserDataAddress > USER_FLASH_FIRST_PAGE_ADDRESS + USERDATA_WORDS_SIZE * MAX_NUMBER_OF_USERDATA_IN_SECTOR){
	//	printf("Sector is full to erase sector!\n");
		UserData_Erase();
		UserDataAddress = USER_FLASH_FIRST_PAGE_ADDRESS;
	}
	// Write settings
	UserData.Used = FLASH_USERDATA_VALID;	//initialize valid block
	uint32_t *source_addr = (void *)&UserData;
	uint32_t dest_addr = UserDataAddress;
	for (uint16_t i = 0; i < USERDATA_WORDS_SIZE; i++) {
		if( FLASH_ProgramWord(dest_addr, *source_addr) == FLASH_COMPLETE){
		//	printf("FLASH_COMPLETE\n");
			if( *((uint32_t*)dest_addr) != *source_addr){
		//		printf("ERROR\n");
				FLASH_Lock();
				return ERROR;
			}
		}
		else {
		  /* Error occurred while writing data in Flash memory.
			 User can add here some code to deal with this error */
	//		printf("ERROR2\n");
			FLASH_Lock();
			return ERROR;
		}
		source_addr++;
		dest_addr += WORD_SIZE;
	}
//	printf("write success\n");
	return SUCCESS;
}

static void UserData_Read( void ){
	//Read settings
	uint32_t *source_addr = (uint32_t *)UserDataAddress;
	uint32_t *dest_addr = (void *)&UserData;
	for (uint16_t i=0; i < USERDATA_WORDS_SIZE; i++) {
		*dest_addr = *(__IO uint32_t*)source_addr;
		source_addr++;
		dest_addr++;
	}
}

static void UserData_Erase(){
	/* Enable the flash control register access */
	FLASH_Unlock();

	FLASH_EraseSector(FLASH_Sector_5, VoltageRange_4);

	FLASH_Lock();
}

