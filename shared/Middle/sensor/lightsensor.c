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
#include "lightsensor.h"
#include "serial.h"
#include "stm32f401re_adc.h"
#include "stm32f401re_dma.h"
#include "stm32f401re_gpio.h"
#include "stm32f401re_rcc.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static __IO uint16_t uhADCConvertedValue = 0;
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
 * @func   LightSensor_Init
 * @brief  Initializes module light sensor
 * @param  None
 * @retval None
 */
void
LightSensor_Init(
	ADC_READ_MODE adc_read_mode
) {
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/* Enable peripheral clocks ***********************************************/
	RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);

	/* ADC Deinitialization ***************************************************/
	ADC_DeInit();

	/* ADC Common Init ********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init **************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADCx_SENSOR, &ADC_InitStructure);

	if (adc_read_mode == ADC_READ_MODE_POLLING)
	{
		ADC_EOCOnEachRegularChannelCmd(ADCx_SENSOR, ENABLE);
		ADC_ContinuousModeCmd(ADCx_SENSOR, DISABLE);
		ADC_DiscModeChannelCountConfig(ADCx_SENSOR, 1);
		ADC_DiscModeCmd(ADCx_SENSOR, ENABLE);
	}
	else
	{
		/* DMA2_Stream0 channel0 configuration ********************************/
		DMA_DeInit(DMA2_Stream0);
		DMA_InitStructure.DMA_Channel = DMA_CHANNELx;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADCx_DR_ADDRESS;
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCConvertedValue;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = 1;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA_STREAMx, &DMA_InitStructure);

		/* DMA2_Stream0 enable */
		DMA_Cmd(DMA_STREAMx, ENABLE);

		/* Enable DMA request after last transfer (Single-ADC mode) */
		ADC_DMARequestAfterLastTransferCmd(ADCx_SENSOR, ENABLE);

		/* Enable ADC1 DMA */
		ADC_DMACmd(ADCx_SENSOR, ENABLE);
	}

	/* ADC1 regular channel15 configuration ************************************/
	ADC_RegularChannelConfig(ADCx_SENSOR, ADC_Channel_15, 1, ADC_SampleTime_15Cycles);

	/* Enable ADC1 */
	ADC_Cmd(ADCx_SENSOR, ENABLE);

	if (adc_read_mode == ADC_READ_MODE_DMA)
	{
		/* Start ADC1 Software Conversion */
		ADC_SoftwareStartConv(ADCx_SENSOR);
	}
}

/**
 * @func   LightSensor_MeasureUseDMAMode
 * @brief  Measure value ADC in mode DMA
 * @param  None
 * @retval Value of ADC
 */
uint16_t
LightSensor_MeasureUseDMAMode(void) {
	return uhADCConvertedValue;
}

/**
 * @func   LightSensor_MeasureUsePollingMode
 * @brief  Measure value ADC in mode polling
 * @param  None
 * @retval Value of ADC
 */
uint16_t
LightSensor_MeasureUsePollingMode(void)
{
	uint16_t result = 0;

	// Start ADCx software conversion
	ADC_SoftwareStartConv(ADCx_SENSOR);

	// Wait for ADC conversion complete
	while (ADC_GetFlagStatus(ADCx_SENSOR, ADC_FLAG_EOC) == RESET);

	// Read value
	result = ADC_GetConversionValue(ADCx_SENSOR);

	return result;
}

/**
 * @func   LightSensor_SendPacketRespond
 * @brief  Respond frame value light
 * @param  value: value of light
 * @retval None
 */
void
LightSensor_SendPacketRespond(
	uint16_t value
) {
	uint8_t byPayload[CMD_SIZE_OF_PAYLOAD_LIGHTSEN];

	byPayload[0] = (value >> 8) & 0xFFU;
	byPayload[1] = value & 0xFFU;

	/* Send message uart button press 1 time */
	Serial_SendPacket(CMD_OPT_NOT_USE,
					  CMD_ID_LIGHT_SENSOR,
					  CMD_TYPE_RES,
					  byPayload,
					  sizeof(byPayload));
}

/* END FILE */
