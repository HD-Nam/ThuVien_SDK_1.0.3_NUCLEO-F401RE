/*

  Ucglib.cpp

  ucglib = universal color graphics library
  ucglib = micro controller graphics library
  
  Universal uC Color Graphics Library
  
  Copyright (c) 2014, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of s_t ource code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/

#include "Ucglib.h"
#include "stm32f401re_rcc.h"
#include "stm32f401re_gpio.h"

#define SPI1_CS_PORT            GPIOB
#define SPI1_CS_PIN             GPIO_Pin_6
#define SPI1_RST_PORT           GPIOC
#define SPI1_RST_PIN            GPIO_Pin_7
#define SPI1_MOSI_PORT          GPIOA
#define SPI1_MOSI_PIN           GPIO_Pin_7
#define SPI1_SCK_PORT           GPIOA
#define SPI1_SCK_PIN            GPIO_Pin_5
#define SPI1_RS_PORT            GPIOA
#define SPI1_RS_PIN             GPIO_Pin_9
#define SPI1_ENABLE_PORT        GPIOB
#define SPI1_ENABLE_PIN         GPIO_Pin_10
#define SPI1_MODE_PORT          GPIOA
#define SPI1_MODE_PIN           GPIO_Pin_8

static ucg_int_t tx, ty;          // current position for the Print base class procedures
static uint8_t tdir;

//uint8_t get_tdir(void) { return tdir; };
//ucg_int_t get_tx(void) { return tx; };
//ucg_int_t get_ty(void) { return ty; };
//ucg_t *get_ucg(void) { return &ucg; };

static void delayMicroseconds(uint32_t microseconds)
{
//	uint32_t release = GetMilSecTick() + microseconds;
//
//	while (1) {
//		if (release < GetMilSecTick()) break;
//	}
//	uint32_t i;
//	for( i = 0; i < microseconds; i++ )
//	{
//	}
}

static void ucg_gpio_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA, GPIOB and GPIOC Clocks enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN | SPI1_MOSI_PIN | SPI1_RS_PIN | SPI1_MODE_PIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_CS_PIN | SPI1_ENABLE_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_RST_PIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void ucg_com_arduino_send_generic_SW_SPI(ucg_t *ucg, uint8_t data)
{
  uint8_t i = 8;

  do
  {
    if ( data & 128 )
    {
      GPIO_WriteBit(SPI1_MOSI_PORT, SPI1_MOSI_PIN, 1);
    }
    else
    {
      GPIO_WriteBit(SPI1_MOSI_PORT, SPI1_MOSI_PIN, 0);
    }
    // no delay required, also Arduino Due is slow enough
    // delay required for ESP32
    delayMicroseconds(1);
    GPIO_WriteBit(SPI1_SCK_PORT, SPI1_SCK_PIN, 1);
    delayMicroseconds(1);
    i--;
    GPIO_WriteBit(SPI1_SCK_PORT, SPI1_SCK_PIN, 0);
    delayMicroseconds(1);
    data <<= 1;
  } while( i > 0 );
}

static int16_t ucg_com_arduino_generic_SW_SPI(ucg_t *ucg, int16_t msg, uint16_t arg, uint8_t *data)
{
  switch(msg)
  {
    case UCG_COM_MSG_POWER_UP:
      /* "data" is a pointer to ucg_com_info_t structure with the following information: */
      /*	((ucg_com_info_t *)data)->serial_clk_speed value in nanoseconds */
      /*	((ucg_com_info_t *)data)->parallel_clk_speed value in nanoseconds */
    
      /* setup pins */
      ucg_gpio_init();

      GPIO_WriteBit(SPI1_MOSI_PORT, SPI1_MOSI_PIN, 1);
      GPIO_WriteBit(SPI1_CS_PORT, SPI1_CS_PIN, 1);
      GPIO_WriteBit(SPI1_RST_PORT, SPI1_RST_PIN, 1);
      GPIO_WriteBit(SPI1_RS_PORT, SPI1_RS_PIN, 1);
      GPIO_WriteBit(SPI1_SCK_PORT, SPI1_SCK_PIN, 0);
      GPIO_WriteBit(SPI1_ENABLE_PORT, SPI1_ENABLE_PIN, 1);
      GPIO_WriteBit(SPI1_MODE_PORT, SPI1_MODE_PIN, 1);
      break;

    case UCG_COM_MSG_POWER_DOWN:
      break;

    case UCG_COM_MSG_DELAY:
      delayMicroseconds(arg);
      break;

    case UCG_COM_MSG_CHANGE_RESET_LINE:
      GPIO_WriteBit(SPI1_RST_PORT, SPI1_RST_PIN, arg);
      break;

    case UCG_COM_MSG_CHANGE_CS_LINE:
	  GPIO_WriteBit(SPI1_CS_PORT, SPI1_CS_PIN, arg);
      break;

    case UCG_COM_MSG_CHANGE_CD_LINE:
      GPIO_WriteBit(SPI1_RS_PORT, SPI1_RS_PIN, arg);
      break;

    case UCG_COM_MSG_SEND_BYTE:
      ucg_com_arduino_send_generic_SW_SPI(ucg, arg);
      break;

    case UCG_COM_MSG_REPEAT_1_BYTE:
      while( arg > 0 ) {
	    ucg_com_arduino_send_generic_SW_SPI(ucg, data[0]);
	    arg--;
      }
      break;

    case UCG_COM_MSG_REPEAT_2_BYTES:
      while( arg > 0 ) {
		ucg_com_arduino_send_generic_SW_SPI(ucg, data[0]);
		ucg_com_arduino_send_generic_SW_SPI(ucg, data[1]);
		arg--;
      }
      break;

    case UCG_COM_MSG_REPEAT_3_BYTES:
      while( arg > 0 ) {
		ucg_com_arduino_send_generic_SW_SPI(ucg, data[0]);
		ucg_com_arduino_send_generic_SW_SPI(ucg, data[1]);
		ucg_com_arduino_send_generic_SW_SPI(ucg, data[2]);
		arg--;
      }
      break;

    case UCG_COM_MSG_SEND_STR:
      while( arg > 0 ) {
		ucg_com_arduino_send_generic_SW_SPI(ucg, *data++);
		arg--;
      }
      break;

    case UCG_COM_MSG_SEND_CD_DATA_SEQUENCE:
      while(arg > 0)
      {
		if ( *data != 0 )
		{
		  if ( *data == 1 )
		  {
			GPIO_WriteBit(SPI1_RS_PORT, SPI1_RS_PIN, 0);
		  }
		  else
		  {
			GPIO_WriteBit(SPI1_RS_PORT, SPI1_RS_PIN, 1);
		  }
		}
		data++;
		ucg_com_arduino_send_generic_SW_SPI(ucg, *data);
		data++;
		arg--;
      }
      break;
  }
  return 1;
}

void Ucglib4WireSWSPI_begin(ucg_t *ucg, uint8_t is_transparent)
{ 
  ucg_Init(ucg, ucg_dev_st7735_18x128x128, ucg_ext_st7735_18, ucg_com_arduino_generic_SW_SPI);
  ucg_SetFontMode(ucg, is_transparent);
}

/*=========================================================================*/

void Ucglib_init(ucg_t *ucg) {
  // do a dummy init so that something usefull is part of the ucg structure
  ucg_Init(ucg, ucg_dev_default_cb, ucg_ext_none, (ucg_com_fnptr)0);

  // reset cursor position
  tx = 0;
  ty = 0;
  tdir = 0;	// default direction for Arduino print() 
}

//size_t Ucglib_write(uint8_t c) {
////  ucg_int_t delta;
////  delta = ucg_DrawGlyph(get_ucg(), get_tx(), get_ty(), get_tdir(), c);
////  switch(get_tdir()) {
////    case 0: get_tx() += delta; break;
////    case 1: get_ty() += delta; break;
////    case 2: get_tx() -= delta; break;
////    default: case 3: get_ty() -= delta; break;
////  }
//  return 1;
//}

