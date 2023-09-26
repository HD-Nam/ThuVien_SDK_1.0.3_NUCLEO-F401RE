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
#include "temhumsensor.h"
#include "serial.h"
#include "stm32f401re_rcc.h"
#include "stm32f401re_i2c.h"
#include "stm32f401re_gpio.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static uint8_t g_byTimeWaitGetTemp = 4;
static uint8_t g_byTimeWaitGetHumi = 8;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static uint8_t CalculateCRC8(uint8_t *pByData, uint8_t byLength);
void TemHumSensor_SetAdcResolution(uint8_t byAdcRes);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   TemHumSensor_Init
 * @brief  Initialize module SI7020
 * @param  None
 * @retval None
 */
void TemHumSensor_Init(void)
{
    #ifdef SI7020_DEBUG
    BYTE pReg[8];
    BYTE CMDR_USERREG1[2] = { 2, 0xE7 };
    BYTE CMDR_HEATERCTR[2] = { 2, 0x11 };
    #endif

    /* Initialized module i2c */
    i2c_config();

//    TemHumSensor_SetAdcResolution(ADC_RES_RH11_T11);

    #ifdef SI7020_DEBUG
    i2c_read_reg_expand(SI7020_ADDR, CMDR_USERREG1, pReg, 1, 0);
    DBG_SI7020_SEND_STR("$ SI7020 UREG1: ");
    DBG_SI7020_SEND_HEX(pReg[0]);
    DBG_SI7020_SEND_STR("\n");

    i2c_read_reg_expand(SI7020_ADDR, CMDR_HEATERCTR, pReg, 1, 0);
    DBG_SI7020_SEND_STR("$ SI7020 HEATCTR: ");
    DBG_SI7020_SEND_HEX(pReg[0]);
    DBG_SI7020_SEND_STR("\n");

    Si7020GetVerFW();

    Si7020GetSerialNum(pReg);
    #endif /* SI7020_DEBUG */
}

/**
 * @func   TemHumSensor_SetAdcResolution
 * @brief  Set resolution adc
 * @param  byAdcRes
 * @retval None
 */
void TemHumSensor_SetAdcResolution(uint8_t byAdcRes)
{
	uint8_t CMDW_USERREG1[2] = {2, 0xE6};
	uint8_t pByUserReg[1] = {0};

    switch(byAdcRes) {
        case ADC_RES_RH12_T14:
            g_byTimeWaitGetTemp = 15;
            g_byTimeWaitGetHumi = 25;
            pByUserReg[0] = 0x00;
            break;

        case ADC_RES_RH08_T12:
            g_byTimeWaitGetTemp = 3;
            g_byTimeWaitGetHumi = 4;
            pByUserReg[0] = 0x01;
            break;

        case ADC_RES_RH10_T13:
            g_byTimeWaitGetTemp = 10;
            g_byTimeWaitGetHumi = 8;
            pByUserReg[0] = 0x80;
            break;

        case ADC_RES_RH11_T11:
            g_byTimeWaitGetTemp = 4;
            g_byTimeWaitGetHumi = 8;
            pByUserReg[0] = 0x81;
            break;

        default :
            break;
    }

    i2c_read_reg_expand(SI7020_ADDR, CMDW_USERREG1, pByUserReg, 1, 0);
}

/**
 * @func   TemHumSensor_Reset
 * @brief  Reset sensor
 * @param  None
 * @retval None
 */
void TemHumSensor_Reset(void)
{
	uint8_t pData[1];
	uint8_t CMD_RESET = 0xFE;

    i2c_write_multi_with_reg(SI7020_ADDR, CMD_RESET, pData, 0);
}

/**
 * @func    TemHumSensor_GetSerialNum
 * @brief   Get serial number of sensor
 * @param   pbySerial: serial number
 * @retval  None
 */
void
TemHumSensor_GetSerialNum(
	uint8_t *pbySerial
) {
	uint8_t j = 0;
	uint8_t pData[8];
	uint8_t CMD_ELECID1[3] = {3, 0xFA, 0x0F };
	uint8_t CMD_ELECID2[3] = {3, 0xFC, 0xC9 };

    i2c_read_reg_expand(SI7020_ADDR, CMD_ELECID1, pData, 8, 0);
    pbySerial[j++] = pData[0];
    pbySerial[j++] = pData[2];
    pbySerial[j++] = pData[4];
    pbySerial[j++] = pData[6];

    i2c_read_reg_expand(SI7020_ADDR, CMD_ELECID2, pData, 6, 0);
    pbySerial[j++] = pData[0];
    pbySerial[j++] = pData[1];
    pbySerial[j++] = pData[3];
    pbySerial[j++] = pData[4];

    #ifdef SI7020_DEBUG
    DBG_SI7020_SEND_STR("$ SI7020 SERIAL: ");
    for (j = 0; j < 8; j++) {
        DBG_SI7020_SEND_HEX(pbySerial[j]);
        DBG_SI7020_SEND_STR(" ");
    }
    DBG_SI7020_SEND_STR("\n");
    #endif
}

/**
 * @func    TemHumSensor_GetVerFW
 * @brief   Get firmware of sensor
 * @param   None
 * @retval  id firmware
 */
uint8_t
TemHumSensor_GetVerFW(void)
{
	uint8_t data;
	uint8_t CMD_VERFW[3]  = { 3, 0x84, 0xB8 };

    i2c_read_reg_expand(SI7020_ADDR, CMD_VERFW, &data, 1, 0);

    return data;
}

/**
 * @func   TempSensor_SendPacketRespond
 * @brief  Respond frame value temperature
 * @param  value: value of temperature
 * @retval None
 */
void
TempSensor_SendPacketRespond(
	uint16_t value
) {
	uint8_t byPayload[CMD_SIZE_OF_PAYLOAD_TEMPSEN];

	byPayload[0] = (value >> 8) & 0xFFU;
	byPayload[1] = value & 0xFFU;

	/* Send message uart button press 1 time */
	Serial_SendPacket(CMD_OPT_NOT_USE,
					  CMD_ID_TEMP_SENSOR,
					  CMD_TYPE_RES,
					  byPayload,
					  sizeof(byPayload));
}

/**
 * @func   HumiSensor_SendPacketRespond
 * @brief  Respond frame value humidity
 * @param  value: value of humidity
 * @retval None
 */
void
HumiSensor_SendPacketRespond(
	uint16_t value
) {
	uint8_t byPayload[CMD_SIZE_OF_PAYLOAD_HUMISEN];

	byPayload[0] = (value >> 8) & 0xFFU;
	byPayload[1] = value & 0xFFU;

	/* Send message uart button press 1 time */
	Serial_SendPacket(CMD_OPT_NOT_USE,
					  CMD_ID_HUMI_SENSOR,
					  CMD_TYPE_RES,
					  byPayload,
					  sizeof(byPayload));
}

/**
 * @func    TemHumSensor_GetTemp
 * @brief   Get value temperature
 * @param   None
 * @retval  Temperature
 */
uint32_t TemHumSensor_GetTemp(void)
{
	uint32_t wRetval;
    uint8_t pByRetval[3] = { 0 };
    uint8_t byCheckCRC = 0;

    uint8_t CMD_MEASURE_TEMP[2] =  { 2, 0xE3 };

    i2c_read_reg_expand(SI7020_ADDR, CMD_MEASURE_TEMP, pByRetval, 3, g_byTimeWaitGetTemp);
    byCheckCRC = CalculateCRC8(pByRetval, 2);

    /* Check CRC value */
    if (byCheckCRC == pByRetval[2]) {
    }
    else {
        return 0;
    }

    wRetval = (pByRetval[0] << 8) + pByRetval[1];
    wRetval = ((wRetval * 17572) >> 16) - 4685;

    return wRetval;
}

/**
 * @func    TemHumSensor_GetHumi
 * @brief   Get value humidity
 * @param   None
 * @retval  Humidity
 */
uint32_t TemHumSensor_GetHumi(void)
{
	uint32_t wRetval;
	uint8_t pByRetval[3] = { 0 };
	uint8_t byCheckCRC = 0;

	uint8_t CMD_MEASURE_RH[2] =  { 2, 0xE5 };

    i2c_read_reg_expand(SI7020_ADDR, CMD_MEASURE_RH, pByRetval, 3, g_byTimeWaitGetHumi);
    byCheckCRC = CalculateCRC8(pByRetval, 2);

    /* Check CRC value */
    if (byCheckCRC == pByRetval[2]) {
    }
    else {
        return 0;
    }

    wRetval = (pByRetval[0] << 8) + pByRetval[1];
    wRetval = ((wRetval * 12500) >> 16) - 600;

    return wRetval;
}

/**
 * @func   CalculateCRC8
 * @brief  CRC-8 use x^8 + x^5 + x^4 + 1
 * @param  None
 * @retval None
 */
uint8_t CalculateCRC8(uint8_t *pByData, uint8_t byLength)
{
    uint8_t i, j = 0;
    uint16_t wCrc = 0;
    uint8_t *pByCurrData = pByData;

    for (j = 0; j < byLength; j++, pByCurrData++) {
        wCrc ^= ((uint16_t)(*pByCurrData) << 8);
        for (i = 0; i < 8; i++) {
            if ((wCrc & 0x8000) != 0)
                wCrc ^= (0x1310 << 3);
            wCrc <<= 1;
        }
    }
    return ((wCrc >> 8) & 0xFF);
}

/** Public functions -------------------------------------------------------- */
/**
  ******************************************************************************
  *	@brief	Initialize I2C in master mode
  * @param	None
  * @retval	None
  ******************************************************************************
  */
void i2c_config(void)
{
	// Initialization struct
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	// Step 1: Initialize I2C
	RCC_APB1PeriphClockCmd(I2Cx_RCC, ENABLE);
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx_SENSOR, &I2C_InitStruct);
	I2C_Cmd(I2Cx_SENSOR, ENABLE);

	// Step 2: Initialize GPIO as open drain alternate function
	RCC_AHB1PeriphClockCmd(I2C_GPIO_RCC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);

	/* Connect PXx to I2C_SCL */
	GPIO_PinAFConfig(I2C_GPIO, GPIO_PinSource8, GPIO_AF_I2C1);

	/* Connect PXx to I2C_SDA */
	GPIO_PinAFConfig(I2C_GPIO, GPIO_PinSource9, GPIO_AF_I2C1);
}

/**
  ******************************************************************************
  *	@brief	Write byte to slave without specify register address
  * @param	Slave device address (7-bit right aligned)
  * @param	Data byte
  * @retval	None
  ******************************************************************************
  */
void i2c_write_no_reg(uint8_t address, uint8_t data)
{
	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Transmitter);
	i2c_transmit(data);
	i2c_stop();
}

/**
  ******************************************************************************
  *	@brief	Write byte to slave with specify register address
  * @param	Slave device address (7-bit right aligned)
  * @param	Register address
  * @param	Data byte
  * @retval	None
  ******************************************************************************
  */
void i2c_write_with_reg(uint8_t address, uint8_t reg, uint8_t data)
{
	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Transmitter);
	i2c_transmit(reg);
	i2c_transmit(data);
	i2c_stop();
}

/**
  ******************************************************************************
  *	@brief	Write bytes to slave without specify register address where to
  *					start write
  * @param	Slave device address (7-bit right aligned)
  * @param	Pointer to data byte array
  * @param	Number of bytes to write
  * @retval	None
  ******************************************************************************
  */
void i2c_write_multi_no_reg(uint8_t address, uint8_t* data, uint8_t len)
{
	int i;

	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Transmitter);
	for (i = 0; i < len; i++)
	{
		i2c_transmit(data[i]);
	}
	i2c_stop();
}

/**
  ******************************************************************************
  *	@brief	Write bytes to slave with specify register address where to
  *					start write
  * @param	Slave device address (7-bit right aligned)
  * @param	Register address where to start write
  * @param	Pointer to data byte array
  * @param	Number of bytes to write
  * @retval	None
  ******************************************************************************
  */
void i2c_write_multi_with_reg(uint8_t address, uint8_t reg, uint8_t* data, uint8_t len)
{
	int i;

	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Transmitter);
	i2c_transmit(reg);
	for (i = 0; i < len; i++)
	{
		i2c_transmit(data[i]);
	}
	i2c_stop();
}

/**
  ******************************************************************************
  *	@brief	Read byte from slave without specify register address
  * @param	Slave device address (7-bit right aligned)
  * @param	Pointer to data byte to store data from slave
  * @retval	None
  ******************************************************************************
  */
void i2c_read_no_reg(uint8_t address, uint8_t* data)
{
	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Receiver);
	*data = i2c_receive_nack();
	i2c_stop();
}

/**
  ******************************************************************************
  *	@brief	Read byte from slave with specify register address
  * @param	Slave device address (7-bit right aligned)
  * @param	Register address
  * @param	Pointer to data byte to store data from slave
  * @retval	None
  ******************************************************************************
  */
void i2c_read_with_reg(uint8_t address, uint8_t reg, uint8_t* data)
{
	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Transmitter);
	i2c_transmit(reg);
	i2c_stop();
	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Receiver);
	*data = i2c_receive_nack();
	i2c_stop();
}

/**
  ******************************************************************************
  *	@brief	Read bytes from slave without specify register address
  * @param	Slave device address (7-bit right aligned)
  * @param	Number of data bytes to read from slave
  * @param	Pointer to data array byte to store data from slave
  * @retval	None
  ******************************************************************************
  */
void i2c_read_multi_no_reg(uint8_t address, uint8_t len, uint8_t* data)
{
	int i;

	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Receiver);
	for (i = 0; i < len; i++)
	{
		if (i == (len - 1))
		{
			data[i] = i2c_receive_nack();
		}
		else
		{
			data[i] = i2c_receive_ack();
		}
	}
	i2c_stop();
}

/**
  ******************************************************************************
  *	@brief	Read bytes from slave with specify register address
  * @param	Slave device address (7-bit right aligned)
  * @param	Register address
  * @param	Number of data bytes to read from slave
  * @param	Pointer to data array byte to store data from slave
  * @retval	None
  ******************************************************************************
  */
void i2c_read_multi_with_reg(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data)
{
	int i;

	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Transmitter);
	i2c_transmit(reg);
	i2c_stop();
	i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Receiver);
	for (i = 0; i < len; i++)
	{
		if (i == (len - 1))
		{
			data[i] = i2c_receive_nack();
		}
		else
		{
			data[i] = i2c_receive_ack();
		}
	}
	i2c_stop();
}

/**
 * @func   i2c_read_reg_expand
 * @brief  None
 * @param  None
 * @retval None
 */
void
i2c_read_reg_expand(
    uint8_t address,
    uint8_t* pAddressReg,
    uint8_t* pDataRead,
    uint8_t byLengthData,
    uint16_t wDelay
) {
    uint8_t byLenCmd = pAddressReg[0];

    i2c_start();
	i2c_address_direction(address << 1, I2C_Direction_Transmitter);

    for (uint8_t i = 1; i < byLenCmd; i++) {
        i2c_transmit(pAddressReg[i]);
    }

//    if (wDelay > 0) {
//        delay_ms(wDelay);
//    }

	i2c_stop();
	i2c_start();
    i2c_address_direction(address << 1, I2C_Direction_Receiver);

    for (uint8_t i = 0; i < byLengthData; i++)
	{
		if (i == (byLengthData - 1))
		{
			pDataRead[i] = i2c_receive_nack();
		}
		else
		{
			pDataRead[i] = i2c_receive_ack();
		}
	}
	i2c_stop();
}

/** Private functions ------------------------------------------------------- */
/**
  ******************************************************************************
  *	@brief	Generate I2C start condition
  * @param	None
  * @retval	None
  ******************************************************************************
  */
void i2c_start(void)
{
	// Wait until I2Cx is not busy anymore
	while (I2C_GetFlagStatus(I2Cx_SENSOR, I2C_FLAG_BUSY));

	// Generate start condition
	I2C_GenerateSTART(I2Cx_SENSOR, ENABLE);

	// Wait for I2C EV5.
	// It means that the start condition has been correctly released
	// on the I2C bus (the bus is free, no other devices is communicating))
	while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_MODE_SELECT));
}

/**
  ******************************************************************************
  *	@brief	Generate I2C stop condition
  * @param	None
  * @retval	None
  ******************************************************************************
  */
void i2c_stop(void)
{
	// Generate I2C stop condition
	I2C_GenerateSTOP(I2Cx_SENSOR, ENABLE);
}

/**
  ******************************************************************************
  *	@brief	Write slave address to I2C bus
	* @param	Slave address
	* @param	I2C direction (transmitter or receiver)
  * @retval	None
  ******************************************************************************
  */
void i2c_address_direction(uint8_t address, uint8_t direction)
{
	// Send slave address
	I2C_Send7bitAddress(I2Cx_SENSOR, address, direction);

	// Wait for I2C EV6
	// It means that a slave acknowledges his address
	if (direction == I2C_Direction_Transmitter)
	{
		while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if (direction == I2C_Direction_Receiver)
	{
		while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

/**
  ******************************************************************************
  *	@brief	Transmit one byte to I2C bus
  * @param	Data byte to transmit
  * @retval	None
  ******************************************************************************
  */
void i2c_transmit(uint8_t byte)
{
	// Send data byte
	I2C_SendData(I2Cx_SENSOR, byte);
	// Wait for I2C EV8_2.
	// It means that the data has been physically shifted out and
	// output on the bus)
	while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/**
  ******************************************************************************
  *	@brief	Receive data byte from I2C bus, then return ACK
  * @param	None
  * @retval	Received data byte
  ******************************************************************************
  */
uint8_t i2c_receive_ack(void)
{
	// Enable ACK of received data
	I2C_AcknowledgeConfig(I2Cx_SENSOR, ENABLE);
	// Wait for I2C EV7
	// It means that the data has been received in I2C data register
	while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Read and return data byte from I2C data register
	return I2C_ReceiveData(I2Cx_SENSOR);
}

/**
  ******************************************************************************
  *	@brief	Receive data byte from I2C bus, then return NACK
  * @param	None
  * @retval	Received data byte
  ******************************************************************************
  */
uint8_t i2c_receive_nack(void)
{
	// Disable ACK of received data
	I2C_AcknowledgeConfig(I2Cx_SENSOR, DISABLE);
	// Wait for I2C EV7
	// It means that the data has been received in I2C data register
	while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Read and return data byte from I2C data register
	return I2C_ReceiveData(I2Cx_SENSOR);
}

/* END FILE */
