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
#ifndef _TEMHUMSENSOR_H_
#define _TEMHUMSENSOR_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/*
 * A6   A5  A4  A3  A2  A1  A0  R/W
 *  1    0   0   0   0   0   0   0
 */
#define SI7020_ADDR                          0x40
#define CMDR_MEASURE_VALUE                   0xE0
#define CMDW_HEATERCTR                       0x51
#define ADC_RES_RH12_T14                     0
#define ADC_RES_RH08_T12                     1
#define ADC_RES_RH10_T13                     2
#define ADC_RES_RH11_T11                     3
/* Serial Number */
#define ID_SAMPLE                            0xFF
#define ID_SI7013                            0x0D
#define ID_SI7020                            0x14
#define ID_SI7021                            0x15
/* Firmware */
#define FIRMWARE_V11                         0xFF
#define FIRMWARE_V20                         0x20

#define I2Cx_RCC							 RCC_APB1Periph_I2C1
#define I2Cx_SENSOR							 I2C1
#define I2C_GPIO_RCC		    			 RCC_AHB1Periph_GPIOB
#define I2C_GPIO							 GPIOB
#define I2C_PIN_SDA			    		 	 GPIO_Pin_9
#define I2C_PIN_SCL			    			 GPIO_Pin_8
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
 * @func   TemHumSensor_Init
 * @brief  Initialize module SI7020
 * @param  None
 * @retval None
 */
void
TemHumSensor_Init(void);

/**
 * @func    TemHumSensor_GetVerFW
 * @brief   Get firmware of sensor
 * @param   None
 * @retval  id firmware
 */
uint8_t
TemHumSensor_GetVerFW(void);

/**
 * @func    TemHumSensor_GetSerialNum
 * @brief   Get serial number of sensor
 * @param   pbySerial: serial number
 * @retval  None
 */
void
TemHumSensor_GetSerialNum(
    uint8_t *pbySerial
);

/**
 * @func   TempSensor_SendPacketRespond
 * @brief  Respond frame value temperature
 * @param  value: value of temperature
 * @retval None
 */
void
TempSensor_SendPacketRespond(
	uint16_t value
);

/**
 * @func   HumiSensor_SendPacketRespond
 * @brief  Respond frame value humidity
 * @param  value: value of humidity
 * @retval None
 */
void
HumiSensor_SendPacketRespond(
	uint16_t value
);

/**
 * @func    TemHumSensor_GetHumi
 * @brief   Get value humidity
 * @param   None
 * @retval  Humidity
 */
uint32_t
TemHumSensor_GetHumi(void);

/**
 * @func    TemHumSensor_GetTemp
 * @brief   Get value temperature
 * @param   None
 * @retval  Temperature
 */
uint32_t
TemHumSensor_GetTemp(void);

/** Public function prototypes ---------------------------------------------- */
void i2c_config(void);
void i2c_write_no_reg(uint8_t address, uint8_t data);
void i2c_write_with_reg(uint8_t address, uint8_t reg, uint8_t data);
void i2c_write_multi_no_reg(uint8_t address, uint8_t* data, uint8_t len);
void i2c_write_multi_with_reg(uint8_t address, uint8_t reg, uint8_t* data, uint8_t len);
void i2c_read_no_reg(uint8_t address, uint8_t* data);
void i2c_read_with_reg(uint8_t address, uint8_t reg, uint8_t* data);
void i2c_read_multi_no_reg(uint8_t address, uint8_t len, uint8_t* data);
void i2c_read_multi_with_reg(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data);
void i2c_read_reg_expand(uint8_t address, uint8_t* pAddressReg, uint8_t* pDataRead, uint8_t byLengthData, uint16_t wDelay);

void i2c_start(void);
void i2c_stop(void);
void i2c_address_direction(uint8_t address, uint8_t direction);
void i2c_transmit(uint8_t byte);
uint8_t i2c_receive_ack(void);
uint8_t i2c_receive_nack(void);

#endif

/* END FILE */
