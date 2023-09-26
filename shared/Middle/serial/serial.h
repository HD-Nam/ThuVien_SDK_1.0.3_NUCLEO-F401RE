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
 * Last Changed:     $Date: 10/07/20 $
 *
 ******************************************************************************/
#ifndef _SERIAL_H_
#define _SERIAL_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/* --------------------------------RECEIVER------------------------------------
 * Definition state receiver
 * ---------------------------------------------------------------------------*/
#define USART1_IDX						 1
#define USART2_IDX						 0
#define USART_COUNT                      2

#define BAUD9600                		 9600
#define BAUD19200               		 19200
#define BAUD38400               		 38400
#define BAUD57600               		 57600
#define BAUD115200              		 115200

#define NO_PARITY               		 0
#define EVEN_PARITY             		 1
#define ODD_PARITY              		 2

#define ONE_STOP_BIT            		 0
#define TWO_STOP_BIT            		 1

#define USARTx_CLK                       RCC_APB1Periph_USART2
#define USARTx_CLK_INIT                  RCC_APB1PeriphClockCmd
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

#define USARTx_TX_PIN                    GPIO_Pin_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USARTx_TX_SOURCE                 GPIO_PinSource2
#define USARTx_TX_AF                     GPIO_AF_USART2

#define USARTx_RX_PIN                    GPIO_Pin_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USARTx_RX_SOURCE                 GPIO_PinSource3
#define USARTx_RX_AF                     GPIO_AF_USART2

/*! @brief Size of rx buffer */
#define RX_BUFFER_SIZE                      30

/*! @brief Timeout rx */
#define RX_TIMEOUT                          20

/*! @brief Start of frame */
#define FRAME_SOF                           0xB1

/*! @brief Frame ack and nack */
#define FRAME_ACK                           0x06
#define FRAME_NACK                          0x15

/*! @brief Check xor init */
#define CXOR_INIT_VAL                       0xFF

/*! @brief Field lenght */
#define FlenBuf                             (*byRxBuffer)

/*! @brief State receiver */
typedef enum {
    RX_STATE_START_BYTE,
    RX_STATE_DATA_BYTES,
    RX_STATE_CXOR_BYTE
}RX_STATE;

/*! @brief State frame uart */
typedef enum {
    UART_STATE_IDLE,
    UART_STATE_DATA_RECEIVED,
    UART_STATE_ACK_RECEIVED,
    UART_STATE_NACK_RECEIVED,
    UART_STATE_ERROR,
    UART_STATE_RX_TIMEOUT,
} UART_STATE;

typedef struct {
    uint8_t cmdid;
    uint8_t type;
} cmd_common_t;

typedef struct {
    uint8_t cmdid;
    uint8_t type;
    uint8_t epoint;
    uint8_t state;
} cmd_button_state_t;

typedef struct {
    uint8_t cmdid;
    uint8_t type;
    uint8_t state;
} cmd_buzzer_state_t;

typedef struct {
    uint8_t cmdid;
    uint8_t type;
    uint8_t text[20];
} cmd_lcd_display_t;

typedef struct {
	uint8_t cmdid;
	uint8_t type;
	uint8_t numID;
	uint8_t color;
	uint8_t counter;
    uint16_t interval;
    uint8_t laststate;
    uint8_t reserved;
} cmd_led_indicator_t;

typedef struct {
    uint8_t cmdid;
    uint8_t type;
    uint8_t mode;
    uint8_t data[2];
} cmd_setup_config_t;

typedef union {
    cmd_common_t          cmdCommon;
    cmd_button_state_t    buttonState;
    cmd_buzzer_state_t    buzzerState;
    cmd_led_indicator_t   ledIndicator;
    cmd_lcd_display_t     lcdDisplay;
    cmd_setup_config_t    setConfig;
} cmd_receive_t, *cmd_receive_p;

typedef void (* serial_handle_event)(void *);
/* -------------------------------TRANSMITER-----------------------------------
 * Definition state transmitter and fields of frame
 * ---------------------------------------------------------------------------*/
#define CMD_OPT_NOT_USE                         0x00
#define CMD_OPT_EXISTCMD_MASK                   0x01
#define CMD_LENGTH_MAX							50 // bytes

/*! @brief Field type */
#define CMD_TYPE_GET                            0x00
#define CMD_TYPE_RES                            0x01
#define CMD_TYPE_SET                            0x02

/*! @brief Field command id */
#define CMD_ID_DEVICE							0x00
#define CMD_ID_LED 						 		0x01
#define CMD_ID_BUZZER							0x04
#define CMD_ID_BUTTON 							0x82
#define CMD_ID_TEMP_SENSOR 						0x84
#define CMD_ID_HUMI_SENSOR 						0x85
#define CMD_ID_LIGHT_SENSOR 					0x86
#define CMD_ID_LCD								0x87

/*! @brief Size of payload field */
#define CMD_SIZE_OF_PAYLOAD_BUTTON              2
#define CMD_SIZE_OF_PAYLOAD_SET_LED             5
#define CMD_SIZE_OF_PAYLOAD_RES_LED             3
#define CMD_SIZE_OF_PAYLOAD_BUZZER              1
#define CMD_SIZE_OF_PAYLOAD_TEMPSEN             2
#define CMD_SIZE_OF_PAYLOAD_HUMISEN             2
#define CMD_SIZE_OF_PAYLOAD_LIGHTSEN            2
#define CMD_SIZE_OF_PAYLOAD_LCD                 3
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
 * @func   Serial_Init
 * @brief  Initialize UART and buffer
 * @param  None
 * @retval None
 */
void
Serial_Init(void);

/**
 * @func   SerialHandleEventCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
SerialHandleEventCallback(
	serial_handle_event pSerialEvent
);

/**
 * @func   SendACK
 * @brief  Send message ACK
 * @param  None
 * @retval None
 */
void
SendACK(void);

/**
 * @func   SendNACK
 * @brief  Send message NACK
 * @param  None
 * @retval None
 */
void
SendNACK(void);

/**
 * @func   procSerialReceiver
 * @brief  Process data received from uart
 * @param  None
 * @retval None
 */
void
processSerialReceiver(void);

/**
 * @func   Serial_SendPacket
 * @brief  Process transmit message uart
 * @param  byOption: Option
 * @param  byCmdId: Identify
 * @param  byType: Type
 * @param  pPayload: Payload
 * @param  byLengthPayload: Length payload
 * @retval None
 */
void
Serial_SendPacket(
    uint8_t byOption,
    uint8_t byCmdId,
    uint8_t byType,
    uint8_t *pPayload,
    uint8_t byLengthPayload
);

#endif

/* END FILE */
