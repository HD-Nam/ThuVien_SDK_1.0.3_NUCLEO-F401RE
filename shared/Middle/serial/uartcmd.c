/******************************************************************************
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
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 10/07/20 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "uartcmd.h"
#include "serial.h"
#include "led.h"
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/* Definition debug event */
#ifdef UARTCMD_DEBUG
#define DBG_UARTCMD_SEND_STR(x)         Debug_SendStr(x)
#define DBG_UARTCMD_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_UARTCMD_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_UARTCMD_SEND_STR(x)
#define DBG_UARTCMD_SEND_NUM(x)
#define DBG_UARTCMD_SEND_HEX(x)
#endif

static buttoncmd_handle_event pButtonHandleEvent;
static buzzercmd_handle_event pBuzzerHandleEvent;
static ledcmd_handle_event pLedHandleEvent;
static lcdcmd_handle_event pLcdHandleEvent;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void procUartCmd(void *pCmd);
static void processUartReceiveCommand_ControlLed(cmd_receive_p pCmd);
static void processUartReceiveCommand_ControlBuzzer(cmd_receive_p pCmd);
static void processUartReceiveCommand_ControlButton(cmd_receive_p pCmd);
static void processUartReceiveCommand_ControlLcd(cmd_receive_p pCmd);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/* -----------------------------------------------------------------------------
 * -------------------------------PROCESS COMMAND-------------------------------
 * ---------------------------------------------------------------------------*/
/**
 * @func   EventSerial_Init
 * @brief  None
 * @param  None
 * @retval None
 */
void EventSerial_Init(void) {
	SerialHandleEventCallback(procUartCmd);
	Serial_Init();
}

/**
 * @func   EventSerial_SetEventLedCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSerial_SetEventLedCallback(
	ledcmd_handle_event pSerialEvent
) {
	pLedHandleEvent = pSerialEvent;
}

/**
 * @func   EventSerial_SetEventButtonCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSerial_SetEventButtonCallback(
	buttoncmd_handle_event pSerialEvent
) {
	pButtonHandleEvent = pSerialEvent;
}

/**
 * @func   EventSerial_SetEventBuzzerCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSerial_SetEventBuzzerCallback(
	buzzercmd_handle_event pSerialEvent
) {
	pBuzzerHandleEvent = pSerialEvent;
}

/**
 * @func   EventSerial_SetEventLcdCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSerial_SetEventLcdCallback(
	lcdcmd_handle_event pSerialEvent
) {
	pLcdHandleEvent = pSerialEvent;
}

/**
 * @func   procUartCmd
 * @brief  Process command uart
 * @param  None
 * @retval None
 */
static void
procUartCmd(
    void *arg
) {
	cmd_receive_p pCmd = (cmd_receive_p)arg;

    switch (pCmd->cmdCommon.cmdid) {
		case CMD_ID_LED:
			processUartReceiveCommand_ControlLed(pCmd);
			break;

		case CMD_ID_BUZZER:
			processUartReceiveCommand_ControlBuzzer(pCmd);
			break;

		case CMD_ID_BUTTON:
			processUartReceiveCommand_ControlButton(pCmd);
			break;

		case CMD_ID_LCD:
			processUartReceiveCommand_ControlLcd(pCmd);
			break;

        default:
            /* Respond frame NACK */
            SendNACK();
            break;
    }
}

/* -----------------------------------------------------------------------------
 * ----------------------------CONTROL LED COMMAND------------------------------
 * ---------------------------------------------------------------------------*/
/**
 * @func   uartReceiveRelay_ControlFan_Set
 * @brief  Process set state fan
 * @param  pCmd
 * @retval None
 */
void
uartReceiveControlLed_Set(
    cmd_receive_p pCmd
) {
    if (pCmd->ledIndicator.numID > LED_ALL_ID) {
        /* Respond frame NACK */
        SendNACK();
        return;
    }
    /* Respond frame ACK */
    SendACK();

    // Handler set led
    if (pLedHandleEvent != NULL) {
    	pLedHandleEvent(pCmd->ledIndicator.numID, \
    					pCmd->ledIndicator.color, \
						pCmd->ledIndicator.counter, \
						pCmd->ledIndicator.interval, \
						pCmd->ledIndicator.laststate);
	}
}

/**
 * @func   processUartReceiveCommand_ControlLed
 * @brief  Process command led
 * @param  pCmd
 * @retval None
 */
static void
processUartReceiveCommand_ControlLed(
    cmd_receive_p pCmd
) {
    switch (pCmd->cmdCommon.type) {
        case CMD_TYPE_SET:
            uartReceiveControlLed_Set(pCmd);
            break;
            
        default:
            /* Respond frame NACK */
            SendNACK();
            break;
    }
}

/* -----------------------------------------------------------------------------
 * ---------------------------CONTROL BUZZER COMMAND----------------------------
 * ---------------------------------------------------------------------------*/
/**
 * @func   uartReceiveControlBuzzer_Set
 * @brief  Process set state fan
 * @param  pCmd
 * @retval None
 */
void
uartReceiveControlBuzzer_Set(
    cmd_receive_p pCmd
) {
    if ((pCmd->buzzerState.state > 100) && (pCmd->buzzerState.state != 0xFFU)) {
        /* Respond frame NACK */
        SendNACK();
        return;
    }
    /* Respond frame ACK */
    SendACK();

    // Handler set buzzer
    if (pBuzzerHandleEvent != NULL) {
    	pBuzzerHandleEvent(pCmd->buzzerState.state);
	}

}

/**
 * @func   processUartReceiveCommand_ControlBuzzer
 * @brief  Process command buzzer
 * @param  pCmd
 * @retval None
 */
static void
processUartReceiveCommand_ControlBuzzer(
    cmd_receive_p pCmd
) {
    switch (pCmd->cmdCommon.type) {
        case CMD_TYPE_SET:
            uartReceiveControlBuzzer_Set(pCmd);
            break;

        default:
            /* Respond frame NACK */
            SendNACK();
            break;
    }
}

/* -----------------------------------------------------------------------------
 * ---------------------------CONTROL BUTTON COMMAND----------------------------
 * ---------------------------------------------------------------------------*/
/**
 * @func   uartReceiveControlButton_Set
 * @brief  Process set button
 * @param  pCmd
 * @retval None
 */
void
uartReceiveControlButton_Set(
    cmd_receive_p pCmd
) {
    if ((pCmd->buttonState.epoint > 5) || (pCmd->buttonState.state > 5)) {
        /* Respond frame NACK */
        SendNACK();
        return;
    }
    /* Respond frame ACK */
    SendACK();

    // Handler set button
    if (pButtonHandleEvent != NULL) {
    	pButtonHandleEvent(pCmd->buttonState.epoint, pCmd->buttonState.state);
	}
}

/**
 * @func   processUartReceiveCommand_ControlButton
 * @brief  Process command button
 * @param  pCmd
 * @retval None
 */
static void
processUartReceiveCommand_ControlButton(
    cmd_receive_p pCmd
) {
    switch (pCmd->cmdCommon.type) {
        case CMD_TYPE_SET:
            uartReceiveControlButton_Set(pCmd);
            break;

        default:
            /* Respond frame NACK */
            SendNACK();
            break;
    }
}

/* -----------------------------------------------------------------------------
 * ----------------------------CONTROL LCD COMMAND------------------------------
 * ---------------------------------------------------------------------------*/
/**
 * @func   uartReceiveControlBuzzer_Set
 * @brief  Process set state fan
 * @param  pCmd
 * @retval None
 */
void
uartReceiveControlLcd_Set(
    cmd_receive_p pCmd
) {
//    if (pCmd->lcdDisplay.text > 100) {
//        /* Respond frame NACK */
//        SendNACK();
//        return;
//    }
//    /* Respond frame ACK */
//    SendACK();

	uint8_t i = 0;
	static char buffer[20];

	memsetl((uint8_t *)buffer, 0, sizeof(buffer));

	while (pCmd->lcdDisplay.text[i] != 0x0D)
	{
		buffer[i] = pCmd->lcdDisplay.text[i];
		i++;
	}

    // Handler set lcd
    if (pLcdHandleEvent != NULL) {
    	pLcdHandleEvent((char *)buffer);
	}
}

/**
 * @func   processUartReceiveCommand_ControlLcd
 * @brief  Process command lcd
 * @param  pCmd
 * @retval None
 */
static void
processUartReceiveCommand_ControlLcd(
    cmd_receive_p pCmd
) {
    switch (pCmd->cmdCommon.type) {
        case CMD_TYPE_SET:
        	uartReceiveControlLcd_Set(pCmd);
            break;

        default:
            /* Respond frame NACK */
            SendNACK();
            break;
    }
}

/* END FILE */
