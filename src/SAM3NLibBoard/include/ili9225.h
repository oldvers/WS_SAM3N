/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/**
 * \file
 *
 * Interface of ILI9225 driver.
 *
 */

#ifndef _ILI9225_H
#define _ILI9225_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "types.h"
#include "board.h"

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

//#include <include/lcd_color.h>
//typedef uint16_t LcdColor_t;
/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

//#define ILI9225_RESET_Hi()      
//#define ILI9225_RESET_Lo()      

//#define ILI9225_CS_Hi()         
//#define ILI9225_CS_Lo()         

#define ILI9225_WIDTH                   (176)
#define ILI9225_HEIGHT                  (220)

#define ILI9225_REG_DRVOUTCTRL          (U8)0x01

#define ILI9225_DRIVER_OUTPUT_CTRL      (0x01)  // Driver Output Control
#define ILI9225_LCD_AC_DRIVING_CTRL     (0x02)  // LCD AC Driving Control
#define ILI9225_ENTRY_MODE            	(0x03)  // Entry Mode
#define ILI9225_DISP_CTRL1          	  (0x07)  // Display Control 1
#define ILI9225_DISP_CTRL2		      		(0x08)  // Blank Period Control
#define ILI9225_FRAME_CYCLE_CTRL        (0x0B)  // Frame Cycle Control
#define ILI9225_INTERFACE_CTRL          (0x0C)  // Interface Control
#define ILI9225_OSC_CTRL               	(0x0F)  // Osc Control
#define ILI9225_POWER_CTRL1            	(0x10)  // Power Control 1
#define ILI9225_POWER_CTRL2           	(0x11)  // Power Control 2
#define ILI9225_POWER_CTRL3            	(0x12)  // Power Control 3
#define ILI9225_POWER_CTRL4            	(0x13)  // Power Control 4
#define ILI9225_POWER_CTRL5            	(0x14)  // Power Control 5
#define ILI9225_VCI_RECYCLING          	(0x15)  // VCI Recycling
#define ILI9225_RAM_ADDR_SET1           (0x20)  // Horizontal GRAM Address Set
#define ILI9225_RAM_ADDR_SET2           (0x21)  // Vertical GRAM Address Set
#define ILI9225_GRAM_DATA_REG           (0x22)  // GRAM Data Register
#define ILI9225_GATE_SCAN_CTRL          (0x30)  // Gate Scan Control Register
#define ILI9225_VERTICAL_SCROLL_CTRL1   (0x31)  // Vertical Scroll Control 1 Register
#define ILI9225_VERTICAL_SCROLL_CTRL2   (0x32)  // Vertical Scroll Control 2 Register
#define ILI9225_VERTICAL_SCROLL_CTRL3   (0x33)  // Vertical Scroll Control 3 Register
#define ILI9225_PARTIAL_DRIVING_POS1    (0x34)  // Partial Driving Position 1 Register
#define ILI9225_PARTIAL_DRIVING_POS2    (0x35)  // Partial Driving Position 2 Register
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 (0x36)  // Horizontal Address Start Position
#define ILI9225_HORIZONTAL_WINDOW_ADDR2	(0x37)  // Horizontal Address End Position
#define ILI9225_VERTICAL_WINDOW_ADDR1   (0x38)  // Vertical Address Start Position
#define ILI9225_VERTICAL_WINDOW_ADDR2   (0x39)  // Vertical Address End Position
#define ILI9225_GAMMA_CTRL1            	(0x50)  // Gamma Control 1
#define ILI9225_GAMMA_CTRL2             (0x51)  // Gamma Control 2
#define ILI9225_GAMMA_CTRL3            	(0x52)  // Gamma Control 3
#define ILI9225_GAMMA_CTRL4            	(0x53)  // Gamma Control 4
#define ILI9225_GAMMA_CTRL5            	(0x54)  // Gamma Control 5
#define ILI9225_GAMMA_CTRL6            	(0x55)  // Gamma Control 6
#define ILI9225_GAMMA_CTRL7            	(0x56)  // Gamma Control 7
#define ILI9225_GAMMA_CTRL8            	(0x57)  // Gamma Control 8
#define ILI9225_GAMMA_CTRL9             (0x58)  // Gamma Control 9
#define ILI9225_GAMMA_CTRL10            (0x59)  // Gamma Control 10

#define ILI9225_DRVOUTCTRL_SM           (U16)(0x0400)
#define ILI9225_DRVOUTCTRL_GS           (U16)(0x0200)
#define ILI9225_DRVOUTCTRL_SS           (U16)(0x0100)
#define ILI9225_DRVOUTCTRL_NL           (U16)(0x001F)

//extern uint32_t LCD_Initialize( void ) ;
//extern void LCD_On( void ) ;
//extern void LCD_Off( void ) ;
//extern uint32_t LCD_SetColor(uint32_t color);
//extern uint32_t LCD_DrawPixel(uint32_t x, uint32_t y);
//extern uint32_t LCD_DrawLine ( uint32_t dwX1, uint32_t dwY1, uint32_t dwX2, uint32_t dwY2 );
//extern uint32_t LCD_DrawCircle( uint32_t x, uint32_t y, uint32_t r );
//extern uint32_t LCD_DrawFilledCircle( uint32_t dwX, uint32_t dwY, uint32_t dwRadius);
//extern uint32_t LCD_DrawRectangle( uint32_t dwX1, uint32_t dwY1, uint32_t dwX2, uint32_t dwY2 );
//extern uint32_t LCD_DrawFilledRectangle( uint32_t dwX1, uint32_t dwY1, uint32_t dwX2, uint32_t dwY2 );
//extern uint32_t LCD_DrawPicture( uint32_t dwX1, uint32_t dwY1, uint32_t dwX2, uint32_t dwY2, const LcdColor_t *pBuffer );
//extern void LCD_SetBacklight (uint32_t level);

void ILI9225_Init(void);
void ILI9225_Reset_Lo(void);
void ILI9225_Reset_Hi(void);
void ILI9225_PutData(U16 * aData, U32 aSize, U8 aIncrement);
//void ILI9225_PutCommand(U8 aCommand, U16 * aWParam, U32 aWSize);
void ILI9225_PutCommand(U8 aCommand, U16 aParam, U8 aWithParam);
void ILI9225_SetBacklight(U32 aLevel);

#endif /* #ifndef ILI9225 */
