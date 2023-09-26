/*******************************************************************************
 *
 * Copyright (c) 2019
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
 * Last Changed:     $Date: 2/12/2019 $
 *
 ******************************************************************************/
#ifndef _LIGHTSENSOR_H_
#define _LIGHTSENSOR_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define ADC_PORT				    GPIOC
#define ADC_PIN             	    GPIO_Pin_5

#define ADCx_SENSOR                	ADC1
#define ADCx_CLK                 	RCC_APB2Periph_ADC1
#define DMA_CHANNELx             	DMA_Channel_0
#define DMA_STREAMx              	DMA2_Stream0
#define ADCx_DR_ADDRESS          	((uint32_t)0x4001204C)

typedef enum {
	ADC_READ_MODE_DMA,
	ADC_READ_MODE_POLLING
} ADC_READ_MODE;
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
 * @func   LightSensor_Init
 * @brief  Initializes module light sensor
 * @param  None
 * @retval None
 */
void
LightSensor_Init(
	ADC_READ_MODE adc_read_mode
);

/**
 * @func   LightSensor_MeasureUseDMAMode
 * @brief  Measure value ADC in mode DMA
 * @param  None
 * @retval Value of ADC
 */
uint16_t
LightSensor_MeasureUseDMAMode(void);

/**
 * @func   LightSensor_MeasureUsePollingMode
 * @brief  Measure value ADC in mode polling
 * @param  None
 * @retval Value of ADC
 */
uint16_t
LightSensor_MeasureUsePollingMode(void);

/**
 * @func   LightSensor_SendPacketRespond
 * @brief  Respond frame value light
 * @param  value: value of light
 * @retval None
 */
void
LightSensor_SendPacketRespond(
	uint16_t value
);

#endif

/* END FILE */
