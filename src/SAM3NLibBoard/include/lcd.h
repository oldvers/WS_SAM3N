#ifndef __LCD_H
#define __LCD_H

#include "ili9225.h"
#include "grlib.h"

#define LCD_ORIENT_000   0
#define LCD_ORIENT_090   1
#define LCD_ORIENT_180   2
#define LCD_ORIENT_270   3

#define LCD_Color565(aR,aG,aB)  (((U16)(aR & 0x1F) << 11) | ((U16)(aG & 0x3F) << 5) | ((U16)aB))

//extern const tDisplay g_sLCD1P8TFT;
extern tDisplay g_sILI9225;

void LCD_Init(void);
void LCD_On(void);
void LCD_Off(void);
void LCD_SetBacklight(U32 aLevel);
void LCD_SetRotation(U8 aOrientation);

U8 LCD_GetWidth(void);
U8 LCD_GetHeight(void);
//void LCD_FillRect(U8 aX, U8 aY, U8 aW, U8 aH, U16 aColor);
//void LCD_PutPixel(U8 aX, U8 aY, U16 aColor);
//void LCD_VLine(U8 aX, U8 aY, U8 aH, U16 aColor);
//void LCD_HLine(U8 aX, U8 aY, U8 aW, U16 aColor);
//void LCD_FillScreen(U16 aColor);
//void LCD_SetRotation(U16 aAngle);

//U8 LCD_DrawChar(U8 aX, U8 aY, U8 aChar, U16 aFG, U16 aBG);
//U8 LCD_GetCharWidth(U8 aChar);
  
#endif //__LCD_H
