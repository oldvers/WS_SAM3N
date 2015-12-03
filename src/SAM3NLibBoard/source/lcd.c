#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"

//*****************************************************************************
//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color.
//
// \return Returns the display-driver specific color.
//
// 24-bit format: XXXX XXXX RRRR RRRR GGGG GGGG BBBB BBBB
// 16-bit format: ---- ---- ---- ---- RRRR RGGG GGGB BBBB
//  8-bit format: ---- ---- ---- ---- ---- ---- RRRG GGBB
//
//
//*****************************************************************************

#define DPYCOLORTRANSLATE16(c)  ((((c) & 0x00f80000) >> 8) |                  \
                                 (((c) & 0x0000fc00) >> 5) |                  \
                                 (((c) & 0x000000f8) >> 3))
#define DPYCOLORTRANSLATE8(c)   ((((c) & 0x00e00000) >> 16) |                 \
                                 (((c) & 0x0000e000) >> 11) |                 \
                                 (((c) & 0x000000c0) >> 6))
#define DPYCOLORTRANSLATE DPYCOLORTRANSLATE16

//*****************************************************************************

U16 LcdTxBuff[5];

U8 LCD_Width       = ILI9225_WIDTH;
U8 LCD_Height      = ILI9225_HEIGHT;
U8 LCD_Orientation = LCD_ORIENT_270;
U8 HS, HE, VS, VE, R1, R2;



void LCD_CheckCoordinates(U16 * pX1, U16 * pY1, U16 * pX2, U16 * pY2)
{
  if(*pX1 > (LCD_Width - 1)) *pX1 = LCD_Width - 1;
  if(*pX2 > (LCD_Width - 1)) *pX2 = LCD_Width - 1;
  if(*pY1 > (LCD_Height - 1)) *pY1 = LCD_Height - 1;
  if(*pY2 > (LCD_Height - 1)) *pY2 = LCD_Height - 1;
  
  //LCD_OrientCoordinates(pX1, pY1);
  //LCD_OrientCoordinates(pX2, pY2);
  
  //if(*pX1 > *pX2) LCD_SwapCoordinates(pX1, pX2);
  //if(*pY1 > *pY2) LCD_SwapCoordinates(pY1, pY2);
}


void LCD_SetWindow(U16 aX1, U16 aY1, U16 aX2, U16 aY2)
{
//  LCD_CheckCoordinates(&aX1, &aY1, &aX2, &aY2);

//  HY1P8TFT_CS_Lo();
//  
//  LcdTxBuff[0] = 0x00;
//  LcdTxBuff[1] = aX1;
//  LcdTxBuff[2] = 0x00;
//  LcdTxBuff[3] = aX2;
//	HY1P8TFT_PutCommand(0x2A, LcdTxBuff, 4, NULL, 0);
//  
//  LcdTxBuff[0] = 0x00;
//  LcdTxBuff[1] = aY1;
//  LcdTxBuff[2] = 0x00;
//  LcdTxBuff[3] = aY2;
//	HY1P8TFT_PutCommand(0x2B, LcdTxBuff, 4, NULL, 0);
//  
//  HY1P8TFT_CS_Hi();
  
  //Determine the refresh window area
  //ILI9225_PutCommand(0x36, aX2, TRUE);
  //ILI9225_PutCommand(0x37, aX1, TRUE);
  //ILI9225_PutCommand(0x38, aY2, TRUE);
  //ILI9225_PutCommand(0x39, aY1, TRUE);
  ILI9225_PutCommand(HE, aX2, TRUE);
  ILI9225_PutCommand(HS, aX1, TRUE);
  ILI9225_PutCommand(VE, aY2, TRUE);
  ILI9225_PutCommand(VS, aY1, TRUE);

  //Set cursor
  //ILI9225_PutCommand(0x20, aX1, TRUE); //Column
  //ILI9225_PutCommand(0x21, aY1, TRUE); //Row
  ILI9225_PutCommand(R1, aX1, TRUE);
  ILI9225_PutCommand(R2, aY1, TRUE);

  //Prepare to write in GRAM
  ILI9225_PutCommand(0x22, 0, FALSE);
}


U8 LCD_GetWidth(void)
{
  return LCD_Width;
}


U8 LCD_GetHeight(void)
{
  return LCD_Height;
}
  
//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ui32Value is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both i16XMin and
//! i16XMax are drawn, aint32_t with i16YMin and i16YMax).
//!
//! \return None.
//
//*****************************************************************************
void LCD_FillRect(U16 aX, U16 aY, U16 aW, U16 aH, U16 aColor)
{
  U16 x, y;
  
  x = aX + aW - 1;
  y = aY + aH - 1;
  
  LCD_CheckCoordinates(&aX, &aY, &x, &y);

  //LCD_SetWindow(aX, aY, aX + aW - 1, aY + aH - 1);
  LCD_SetWindow(aX, aY, x, y);
  
  //HY1P8TFT_CS_Lo();

	//HY1P8TFT_PutCommand(0x2C, NULL, 0, NULL, 0);

  ILI9225_PutData(&aColor, (x - aX + 1) * (y - aY + 1), FALSE);
  
  //HY1P8TFT_CS_Hi();  
}

static void LCD_RectFill(void * pvDisplayData, const tRectangle *pRect, uint32_t ui32Value)
//void LCD_FillRect(U8 aX, U8 aY, U8 aW, U8 aH, U16 aColor)
{
	U16 x, y, w, h;
  //U16 x1, y1, x2, y2, w, h;
  
  //x = aX + aW - 1;
  //y = aY + aH - 1;
  x = pRect->i16XMin;
  y = pRect->i16YMin;
  w = pRect->i16XMax - pRect->i16XMin + 1;
  h = pRect->i16YMax - pRect->i16YMin + 1;
  
//  x1 = pRect->i16XMin;
//  y1 = pRect->i16YMin;
//  x2 = pRect->i16XMax;
//  y2 = pRect->i16YMax;

//  LCD_CheckCoordinates(&x1, &y1, &x2, &y2);

//  LCD_SetWindow(x1, y1, x2, y2);
  
  //HY1P8TFT_CS_Lo();

	//HY1P8TFT_PutCommand(0x2C, NULL, 0, NULL, 0);

//  ILI9225_PutData((U16 *)&ui32Value, w * h, FALSE);
  
  //LCD_CheckCoordinates(&aX, &aY, &x, &y);
  //LCD_CheckCoordinates(&x1, &y1, &x2, &y2);
	//LCD_SetWindow(aX, aY, aX + aW - 1, aY + aH - 1);
  //LCD_SetWindow(x1, y1, x2, y2);
  
  //HY1P8TFT_CS_Lo();

	//HY1P8TFT_PutCommand(0x2C, NULL, 0, NULL, 0);

  //HY1P8TFT_PutData(&aColor, aW * aH, MODE_WORDS, FALSE);
  //HY1P8TFT_PutData((U16 *)&ui32Value, (x2 - x1) * (y2 - y1), MODE_WORDS, FALSE);
  
  //HY1P8TFT_CS_Hi();
  
  LCD_FillRect(x, y, w, h, (U16)ui32Value);
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ui32Value is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static uint32_t LCD_ColorTranslate(void *pvDisplayData, uint32_t ui32Value)
{
  //Translate from a 24-bit RGB color to a 5-6-5 RGB color.
  return(DPYCOLORTRANSLATE(ui32Value));
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.  Since no memory
//! based frame buffer is used for this driver, the flush is a no operation.
//!
//! \return None.
//
//*****************************************************************************
static void LCD_Flush(void *pvDisplayData)
{
  //There is nothing to be done.
}

//*****************************************************************************
//
//! Draws a pixel on the screen
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the pixel.
//! \param i32Y is the Y coordinate of the pixel.
//! \param ui32Value is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void LCD_PixelDraw(void * pvDisplayData, int32_t i32X, int32_t i32Y, uint32_t ui32Value)
//void LCD_PutPixel(U8 aX, U8 aY, U16 aColor)
{
  //LCD_FillRect(aX, aY, 1, 1, aColor);
  LCD_FillRect(i32X, i32Y, 1, 1, (U16)ui32Value);

  //U16 x1, y1, x2, y2, w, h;
  
  //x = aX + aW - 1;
  //y = aY + aH - 1;
//  x = pRect->i16XMin;
//  y = pRect->i16YMin;
//  w = pRect->i16XMax - pRect->i16XMin + 1;
//  h = pRect->i16YMax - pRect->i16YMin + 1;
  
//  x1 = pRect->i16XMin;
//  y1 = pRect->i16YMin;
//  x2 = pRect->i16XMax;
//  y2 = pRect->i16YMax;

//  LCD_CheckCoordinates((U16 *)&i32X, (U16 *)&i32Y, (U16 *)&i32X, (U16 *)&i32Y);

//  LCD_SetWindow((U16)i32X, (U16)i32Y, (U16)i32X, (U16)i32Y);
//  
//  //HY1P8TFT_CS_Lo();

//	//HY1P8TFT_PutCommand(0x2C, NULL, 0, NULL, 0);

//  ILI9225_PutData((U16 *)&ui32Value, 1, FALSE);
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the first pixel.
//! \param i32Y is the Y coordinate of the first pixel.
//! \param i32X0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param i32Count is the number of pixels to draw.
//! \param i32BPP is the number of bits per pixel; must be 1, 4, or 8 optionally
//! ORed with various flags unused by this driver.
//! \param pui8Data is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pui8Palette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void LCD_PixelDrawMultiple(
              void * pvDisplayData, 
              int32_t i32X, int32_t i32Y, int32_t i32X0,
              int32_t i32Count, int32_t i32BPP,
              const uint8_t *pui8Data,
              const uint8_t *pui8Palette)
{
  U32 ui32Byte;
  U16 x1, y1, x2, y2;
  
  x1 = i32X;
  y1 = i32Y;
  x2 = i32X + i32X0;
  y2 = i32Y;

  // Send the column, row commands to the display
  //TFT320x240x16SetXY(i32X, i32Y, i32X + i32X0, i32Y);
  
  LCD_CheckCoordinates(&x1, &y1, &x2, &y2);
  LCD_SetWindow(x1, y1, x2, y2);
  
  //HY1P8TFT_CS_Lo();

	//HY1P8TFT_PutCommand(0x2C, NULL, 0, NULL, 0);  

  //Determine how to interpret the pixel data based on the number of bits per pixel.
  switch(i32BPP & 0xFF)
  {
    //The pixel data is in 1 bit per pixel format
    case 1:
    {
      //Loop while there are more pixels to draw
      while(i32Count)
      {
        //Get the next byte of image data
        ui32Byte = *pui8Data++;

        //Loop through the pixels in this byte of image data
        for(; (i32X0 < 8) && i32Count; i32X0++, i32Count--)
        {
          //Draw this pixel in the appropriate color
          uint16_t ui16BPP = ((uint32_t *)pui8Palette)[(ui32Byte >> (7 - i32X0)) & 1];
          //TFT320x240x16WriteData(&ui16BPP, 1);
          //HY1P8TFT_PutData(&ui16BPP, 1, MODE_WORDS, FALSE);
          ILI9225_PutData(&ui16BPP, 1, FALSE);
        }
        //Start at the beginning of the next byte of image data
        i32X0 = 0;
      }
      //The image data has been drawn.
      break;
    }

    //The pixel data is in 4 bit per pixel format
    case 4:
    {
      // Loop while there are more pixels to draw.  "Duff's device" is
      // used to jump into the middle of the loop if the first nibble of
      // the pixel data should not be used.  Duff's device makes use of
      // the fact that a case statement is legal anywhere within a
      // sub-block of a switch statement.  See
      // http://en.wikipedia.org/wiki/Duff's_device for detailed
      // information about Duff's device.
      switch(i32X0 & 1)
      {
        case 0:
          while(i32Count)
          {
            uint16_t ui16Color;

            // Get the upper nibble of the next byte of pixel data
            // and extract the corresponding entry from the
            // palette.
            ui32Byte = (*pui8Data >> 4) * 3;
            ui32Byte = (*(uint32_t *)(pui8Palette + ui32Byte) & 0x00ffffff);

            // Translate this palette entry and write it to the screen
            ui16Color = DPYCOLORTRANSLATE(ui32Byte);
            //TFT320x240x16WriteData(&ui16Color, 1);
            //HY1P8TFT_PutData(&ui16Color, 1, MODE_WORDS, FALSE);
            ILI9225_PutData(&ui16Color, 1, FALSE);

            //Decrement the count of pixels to draw
            i32Count--;

            //See if there is another pixel to draw.
            if(i32Count)
            {
              case 1:
                // Get the lower nibble of the next byte of pixel
                // data and extract the corresponding entry from
                // the palette.
                ui32Byte = (*pui8Data++ & 15) * 3;
                ui32Byte = (*(uint32_t *)(pui8Palette + ui32Byte) & 0x00ffffff);

                //Translate this palette entry and write it to the screen
                ui16Color = DPYCOLORTRANSLATE(ui32Byte);
                //TFT320x240x16WriteData(&ui16Color, 1);
                //HY1P8TFT_PutData(&ui16Color, 1, MODE_WORDS, FALSE);
                ILI9225_PutData(&ui16Color, 1, FALSE);

                //Decrement the count of pixels to draw
                i32Count--;
            }
          }
      }
      //The image data has been drawn
      break;
    }

    //The pixel data is in 8 bit per pixel format
    case 8:
    {
      //Loop while there are more pixels to draw.
      while(i32Count--)
      {
        uint16_t ui16Color;

        //Get the next byte of pixel data and extract the
        //corresponding entry from the palette.
        ui32Byte = *pui8Data++ * 3;
        ui32Byte = *(uint32_t *)(pui8Palette + ui32Byte) & 0x00ffffff;

        //Translate this palette entry and write it to the screen
        ui16Color = DPYCOLORTRANSLATE(ui32Byte);
        //TFT320x240x16WriteData(&ui16Color, 1);
        //HY1P8TFT_PutData(&ui16Color, 1, MODE_WORDS, FALSE);
        ILI9225_PutData(&ui16Color, 1, FALSE);
      }
      //The image data has been drawn
      break;
    }
  }
  
  //HY1P8TFT_CS_Hi();
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the line.
//! \param i32Y1 is the Y coordinate of the start of the line.
//! \param i32Y2 is the Y coordinate of the end of the line.
//! \param ui32Value is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void LCD_LineDrawV(
              void * pvDisplayData, int32_t i32X, int32_t i32Y1, int32_t i32Y2,
              uint32_t ui32Value)
//void LCD_VLine(U8 aX, U8 aY, U8 aH, U16 aColor)
{
  //LCD_FillRect(aX, aY, 1, aH, aColor);
  LCD_FillRect(i32X, i32Y1, 1, i32Y2 - i32Y1 + 1, (U16)ui32Value);
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X1 is the X coordinate of the start of the line.
//! \param i32X2 is the X coordinate of the end of the line.
//! \param i32Y is the Y coordinate of the line.
//! \param ui32Value is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void LCD_LineDrawH(
              void * pvDisplayData, int32_t i32X1, int32_t i32X2, int32_t i32Y,
              uint32_t ui32Value)
//void LCD_HLine(U8 aX, U8 aY, U8 aW, U16 aColor)
{
  //LCD_FillRect(aX, aY, aW, 1, aColor);
  LCD_FillRect(i32X1, i32Y, i32X2 - i32X1 + 1, 1, (U16)ui32Value);
}


void LCD_FillScreen(U16 aColor)
{
  LCD_FillRect(0, 0, LCD_Width, LCD_Height, aColor);
}


void LCD_SetRotation(U8 aOrientation)
{
	LCD_Orientation = aOrientation % 4;
  
//void ILI9225_t3::setRotation(uint8_t r, uint8_t i)
//{
//rotation = r % 4; // can't be higher than 3
//i = i % 2; // can't be higher than 1
//SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
//writecommand_cont(ILI9225_DRIVER_OUTPUT_CTRL);

  switch(LCD_Orientation)
  {
    case LCD_ORIENT_000:
      ILI9225_PutCommand(0x01, 0x011C, TRUE); //(i ? 0x001C:0x011C);
      //writecommand_cont(ILI9225_ENTRY_MODE);
      ILI9225_PutCommand(0x03, 0x1030, TRUE);
      //SPI.endTransaction();
      HS = ILI9225_HORIZONTAL_WINDOW_ADDR2;
      HE = ILI9225_HORIZONTAL_WINDOW_ADDR1;
      VS = ILI9225_VERTICAL_WINDOW_ADDR2;
      VE = ILI9225_VERTICAL_WINDOW_ADDR1;
      R1 = ILI9225_RAM_ADDR_SET1;
      R2 = ILI9225_RAM_ADDR_SET2;
      LCD_Width  = ILI9225_WIDTH;
      LCD_Height = ILI9225_HEIGHT;
      break;
    case LCD_ORIENT_090:
      ILI9225_PutCommand(0x01, 0x001C, TRUE); //(i ? 0x021C:0x001C);
      ILI9225_PutCommand(0x03, 0x1038, TRUE);
      //SPI.endTransaction();
      HS = ILI9225_VERTICAL_WINDOW_ADDR2;
      HE = ILI9225_VERTICAL_WINDOW_ADDR1;
      VS = ILI9225_HORIZONTAL_WINDOW_ADDR2;
      VE = ILI9225_HORIZONTAL_WINDOW_ADDR1;
      R1 = ILI9225_RAM_ADDR_SET2;
      R2 = ILI9225_RAM_ADDR_SET1;
      LCD_Width  = ILI9225_HEIGHT;
      LCD_Height = ILI9225_WIDTH;
      break;
    case LCD_ORIENT_180:
      ILI9225_PutCommand(0x01, 0x021C, TRUE); //(i ? 0x031C:0x021C);
      ILI9225_PutCommand(0x03, 0x1030, TRUE);
//      SPI.endTransaction();
      HS = ILI9225_HORIZONTAL_WINDOW_ADDR2;
      HE = ILI9225_HORIZONTAL_WINDOW_ADDR1;
      VS = ILI9225_VERTICAL_WINDOW_ADDR2;
      VE = ILI9225_VERTICAL_WINDOW_ADDR1;
      R1 = ILI9225_RAM_ADDR_SET1;
      R2 = ILI9225_RAM_ADDR_SET2;
      LCD_Width  = ILI9225_WIDTH;
      LCD_Height = ILI9225_HEIGHT;
      break;
    case LCD_ORIENT_270:
      ILI9225_PutCommand(0x01, 0x031C, TRUE); //(i ? 0x011C:0x031C);
      ILI9225_PutCommand(0x03, 0x1038, TRUE);
//      SPI.endTransaction();
      HS = ILI9225_VERTICAL_WINDOW_ADDR2;
      HE = ILI9225_VERTICAL_WINDOW_ADDR1;
      VS = ILI9225_HORIZONTAL_WINDOW_ADDR2;
      VE = ILI9225_HORIZONTAL_WINDOW_ADDR1;
      R1 = ILI9225_RAM_ADDR_SET2;
      R2 = ILI9225_RAM_ADDR_SET1;
      LCD_Width  = ILI9225_HEIGHT;
      LCD_Height = ILI9225_WIDTH;
      break;
    default:
      break;
  }
//}

//////	switch(LCD_Orientation)
//////  {
//////	  case LCD_ORIENT_000:
//////      //ILI9225_PutCommand(0x03, 0x003F, TRUE);
//////		  LCD_Width  = ILI9225_WIDTH;
//////		  LCD_Height = ILI9225_HEIGHT;
//////		  break;
//////	  case LCD_ORIENT_090:
//////      //ILI9225_PutCommand(0x03, 0x003F, TRUE);
//////		  LCD_Width  = ILI9225_HEIGHT;
//////		  LCD_Height = ILI9225_WIDTH;
//////		  break;
//////	  case LCD_ORIENT_180:
//////      //ILI9225_PutCommand(0x03, 0xFFFF, TRUE);
//////		  LCD_Width  = ILI9225_WIDTH;
//////		  LCD_Height = ILI9225_HEIGHT;
//////		  break;
//////	  case LCD_ORIENT_270:
//////      //ILI9225_PutCommand(0x03, 0xFFFF, TRUE);
//////		  LCD_Width  = ILI9225_HEIGHT;
//////		  LCD_Height = ILI9225_WIDTH;
//////		  break;
//////	}
  
  
//  U16 Value = 0x1000;
  
//  switch(aAngle)
//  {
//    case 0:
//      //0 Degrees, LtoR, W = 128, H = 160
//      Value |= 0x0038;
//      //LCD_MADCTL_MY | 
//      //LCD_MADCTL_MX | 
//      //LCD_MADCTL_MV | 
//      //LCD_MADCTL_ML | 
//      //LCD_MADCTL_RGB | 
//      //LCD_MADCTL_MH |
//      //  0;
//      LCD_Width  = ILI9225_WIDTH;
//      LCD_Height = ILI9225_HEIGHT;
//      break;
////    case 90:
////      //90 Degrees, LtoR, W = 160, H = 128
////      LcdTxBuff[0] = 
////        LCD_MADCTL_MY | 
////      //LCD_MADCTL_MX | 
////        LCD_MADCTL_MV | 
////      //LCD_MADCTL_ML | 
////      //LCD_MADCTL_RGB | 
////      //LCD_MADCTL_MH |
////        0;
////      LCD_Width = HY1P8TFT_HEIGHT;
////      LCD_Height = HY1P8TFT_WIDTH;
////      break;
////    case 180:
////      //180 Degrees, LtoR, W = 128, H = 160
////      LcdTxBuff[0] = 
////        LCD_MADCTL_MY | 
////        LCD_MADCTL_MX | 
////      //LCD_MADCTL_MV | 
////      //LCD_MADCTL_ML | 
////      //LCD_MADCTL_RGB | 
////      //LCD_MADCTL_MH |
////        0;
////      LCD_Width = HY1P8TFT_WIDTH;
////      LCD_Height = HY1P8TFT_HEIGHT;
////      break;
////    case 270:
////      //270 Degrees, LtoR, W = 160, H = 128
////      LcdTxBuff[0] = 
////      //LCD_MADCTL_MY | 
////        LCD_MADCTL_MX | 
////        LCD_MADCTL_MV | 
////      //LCD_MADCTL_ML | 
////      //LCD_MADCTL_RGB | 
////      //LCD_MADCTL_MH |
////        0;
////      LCD_Width = HY1P8TFT_HEIGHT;
////      LCD_Height = HY1P8TFT_WIDTH;
////      break;
//    default:
//      return;
//  }
////  //HY1P8TFT_CS_Lo();
////  //HY1P8TFT_PutCommand(0x36, LcdTxBuff, 1, NULL, 0);
////  //HY1P8TFT_CS_Hi();
////  LcdTxBuff[0] = 0x1030;
//  ILI9225_PutCommand(0x03, &Value, 1);
}


void LCD_Invert(U8 aInvert)
{
//  HY1P8TFT_CS_Lo();
//  HY1P8TFT_PutCommand(aInvert ? 0x21 : 0x20, NULL, 0, NULL, 0);
//  HY1P8TFT_CS_Hi();
}



//U8 LCD_GetCharWidth(U8 aChar)
//{
//  const FONT_INFO * font = &bookmanOldStyle_20ptFontInfo;
//  if(aChar < font->FirstChar || aChar > font->LastChar)
//    return 0;
//  else
//    return font->FontTable[aChar - font->FirstChar].width;
//}

//U8 LCD_DrawChar(U8 aX, U8 aY, U8 aChar, U16 aFG, U16 aBG)
//{
//  const FONT_INFO * font = &bookmanOldStyle_20ptFontInfo;
//  
//  U8 x, y, x2, y2, inc, mask;
//  U8 width  = font->FontTable[aChar - font->FirstChar].width;
//  U8 height = font->Height;
//  CU8 * ptr;
//  U16 color;

//  if(!font) return 0;
//  if(aChar < font->FirstChar || aChar > font->LastChar) return 0;
//  
//  x = aX;
//  y = aY;
//  x2 = aX + width - 1;
//  y2 = aY + height - 1;

//  LCD_CheckCoordinates(&x, &y, &x2, &y2);
//	LCD_SetWindow(x, y, x2, y2);

//  HY1P8TFT_CS_Lo();
//	HY1P8TFT_PutCommand(0x2C, NULL, 0, NULL, 0);

//  inc = width / 8;
//  if(width % 8 != 0) inc++;
//    
//  
//  for(y = 0; y < height; y++)
//  {

//    ptr = &font->FontBitmaps[font->FontTable[aChar - font->FirstChar].start];
//    //ptr += inc * height + (x / 8);
//    ptr += inc * (height - y - 1);// + (x / 8);
//    
//    for(x = 0; x < width; x++)  
//    {
//      mask = 1 << (7 - (x % 8));
//      
//      
//      if(*(ptr + x / 8) & mask) 
//        //LCD_PutPixel(aX + x, aY + y, aColor);
//        color = aFG;
//        //HY1P8TFT_PutData(&aColor, 1, MODE_WORDS, FALSE);
//      else
//        color = aBG;
//        //LCD_PutPixel(aX + x, aY + y, 0x0000);
//      HY1P8TFT_PutData(&color, 1, MODE_WORDS, FALSE);
//      
//      //ptr += x / 8;
//    }
//  }
//  HY1P8TFT_CS_Hi();
//  
//  LCD_FillRect(aX + width, aY, font->FontSpace, height, aBG);

//  return width + font->FontSpace;
//}

void LCD_On(void)
{
  //LcdTxBuff[0] = 0x1017;
  ILI9225_PutCommand(0x07, 0x1017, TRUE);
  //WriteReg(0x07, 0x1017);
}

void LCD_Off(void)
{
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x07, 0x0000, TRUE);
  //WriteReg(0x07, 0x0000);
}

void LCD_SetBacklight(U32 aLevel)
{
  ILI9225_SetBacklight(aLevel);
}
  

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the SSD1332 display controller on the panel,
//! preparing it to display data.
//!
//! \return None.
//
//*****************************************************************************
void LCD_Init(void)
{
  ILI9225_Init();
  
  //Reset LCD module
  ILI9225_Reset_Hi();    //PIO_Set(&pinLcdRst);
  //  i = 2000 * (BOARD_MCK / 1000000);    /* wait for at least 2ms */
  vTaskDelay(5);

  ILI9225_Reset_Lo();    //PIO_Clear(&pinLcdRst);
  //  i = 20000 * (BOARD_MCK / 1000000);    /* wait for at least 20ms */
  vTaskDelay(20);

  ILI9225_Reset_Hi();    //PIO_Set(&pinLcdRst);
  //  i = 50000 * (BOARD_MCK / 1000000);    /* wait for at least 50ms */
  vTaskDelay(50);

  //Turn Off LCD
  //LcdTxBuff[0] = 0x0000;
  //ILI9225_PutCommand(0x07, LcdTxBuff, 1);
  LCD_Off();
  
  //======== LCD module initial code ========//

  //Start Initial Sequence
  
  //Set SS SM GS and NL bit
  //LcdTxBuff[0] = 0x011C;
  ILI9225_PutCommand(
      ILI9225_REG_DRVOUTCTRL,
      //ILI9225_DRVOUTCTRL_GS | ILI9225_DRVOUTCTRL_GS | ILI9225_DRVOUTCTRL_SS | 0x1C,
      ILI9225_DRVOUTCTRL_SS | 0x1C,
      TRUE
      );
  //WriteReg(0x01, 0x011c);

  //Set 1 line inversion
  //LcdTxBuff[0] = 0x0100;
  ILI9225_PutCommand(0x02, 0x0100, TRUE);
  //WriteReg(0x02, 0x0100);

//******************
  //Entry Mode set GRAM write direction and BGR = 1
  //LcdTxBuff[0] = 0x1030;
  //LcdTxBuff[0] = 0x1030;
  ILI9225_PutCommand(0x03, 0x1030, TRUE);
  //WriteReg(0x03, 0x1030);

  //Set GRAM Address
  //LcdTxBuff[0] = ILI9225_HEIGHT;
  ILI9225_PutCommand(0x20, ILI9225_HEIGHT, TRUE);
  //WriteReg(0x20, BOARD_LCD_WIDTH);
  
  //Set GRAM Address
  //LcdTxBuff[0] = ILI9225_WIDTH;
  ILI9225_PutCommand(0x21, ILI9225_WIDTH, TRUE);
  //WriteReg(0x21, BOARD_LCD_HEIGHT);
//******************
  
  //Set BP and FP
  //LcdTxBuff[0] = 0x0808;
  ILI9225_PutCommand(0x08, 0x0808, TRUE);
  //WriteReg(0x08, 0x0808);
  
  //RGB Input Interface Control : 16-bit RGB interface
  //LcdTxBuff[0] = 0x0001;
  ILI9225_PutCommand(0x0C, 0x0001, TRUE);
  //WriteReg(0x0C, 0x0001);
  
  //Set frame rate: 83 Hz
  //LcdTxBuff[0] = 0x0A01;
  ILI9225_PutCommand(0x0F, 0x0A01, TRUE);
  //WriteReg(0x0F, 0x0A01);

////******************
//  //Set GRAM Address
//  LcdTxBuff[0] = ILI9225_WIDTH;
//  ILI9225_PutCommand(0x20, LcdTxBuff, 1);
//  //WriteReg(0x20, BOARD_LCD_WIDTH);
//  
//  //Set GRAM Address
//  LcdTxBuff[0] = ILI9225_HEIGHT;
//  ILI9225_PutCommand(0x21, LcdTxBuff, 1);
//  //WriteReg(0x21, BOARD_LCD_HEIGHT);
////******************

  //Power on sequence
  
  //Set asp DSTB, STB
  //LcdTxBuff[0] = 0x0A00;
  ILI9225_PutCommand(0x10, 0x0A00, TRUE);
  //WriteReg(0x10, 0x0A00);
  
  //SET APON PON AON VCI1EN VC
  //LcdTxBuff[0] = 0x1038;
  ILI9225_PutCommand(0x11, 0x1038, TRUE);
  //WriteReg(0x11, 0x1038);
  
  //Wait for at least 50ms
  vTaskDelay(50);

  //INTERNAL REFERENCE VOLTATE = VCI
  //LcdTxBuff[0] = 0x1121;
  ILI9225_PutCommand(0x12, 0x1121, TRUE);
  //WriteReg(0x12, 0x1121);

  //SET GVDD
  //LcdTxBuff[0] = 0x06CE;
  ILI9225_PutCommand(0x13, 0x06CE, TRUE);
  //WriteReg(0x13, 0x06CE);
  
  //SET VCOMH/VCOML VOLTAGE
  //LcdTxBuff[0] = 0x676F;
  ILI9225_PutCommand(0x14, 0x676F, TRUE);
  //WriteReg(0x14, 0x676F);

  //SET GRAM AREA
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x30, 0x0000, TRUE);
  //WriteReg(0x30, 0x0000);
  //LcdTxBuff[0] = 0x00DB;
  ILI9225_PutCommand(0x31, 0x00DB, TRUE);
  //WriteReg(0x31, 0x00DB);
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x32, 0x0000, TRUE);
  //WriteReg(0x32, 0x0000);
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x33, 0x0000, TRUE);
  //WriteReg(0x33, 0x0000);
  //LcdTxBuff[0] = 0x00DB;
  ILI9225_PutCommand(0x34, 0x00DB, TRUE);
  //WriteReg(0x34, 0x00DB);
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x35, 0x0000, TRUE);
  //WriteReg(0x35, 0x0000);
  //LcdTxBuff[0] = ILI9225_WIDTH;
  ILI9225_PutCommand(0x36, ILI9225_WIDTH, TRUE);
  //WriteReg(0x36, BOARD_LCD_WIDTH);
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x37, 0x0000, TRUE);
  //WriteReg(0x37, 0x0000);
  //LcdTxBuff[0] = ILI9225_HEIGHT;
  ILI9225_PutCommand(0x38, ILI9225_HEIGHT, TRUE);
  //WriteReg(0x38, BOARD_LCD_HEIGHT);
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x39, 0x0000, TRUE);
  //WriteReg(0x39, 0x0000);

  //Set GAMMA CRUVE
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x50, 0x0000, TRUE);
  //WriteReg(0x50, 0x0000);
  //LcdTxBuff[0] = 0x060A;
  ILI9225_PutCommand(0x51, 0x060A, TRUE);
  //WriteReg(0x51, 0x060A);
  //LcdTxBuff[0] = 0x0D0A;
  ILI9225_PutCommand(0x52, 0x0D0A, TRUE);
  //WriteReg(0x52, 0x0D0A);
  //LcdTxBuff[0] = 0x0303;
  ILI9225_PutCommand(0x53, 0x0303, TRUE);
  //WriteReg(0x53, 0x0303);
  //LcdTxBuff[0] = 0x0A0D;
  ILI9225_PutCommand(0x54, 0x0A0D, TRUE);
  //WriteReg(0x54, 0x0A0D);
  //LcdTxBuff[0] = 0x0A06;
  ILI9225_PutCommand(0x55, 0x0A06, TRUE);
  //WriteReg(0x55, 0x0A06);
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x56, 0x0000, TRUE);
  //WriteReg(0x56, 0x0000);
  //LcdTxBuff[0] = 0x0303;
  ILI9225_PutCommand(0x57, 0x0303, TRUE);
  //WriteReg(0x57, 0x0303);
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x58, 0x0000, TRUE);
  //WriteReg(0x58, 0x0000);
  //LcdTxBuff[0] = 0x0000;
  ILI9225_PutCommand(0x59, 0x0000, TRUE);
  //WriteReg(0x59, 0x0000);


////******************
//  //Entry Mode set GRAM write direction and BGR = 1
//  //LcdTxBuff[0] = 0x1030;
//  LcdTxBuff[0] = 0x1000;
//  ILI9225_PutCommand(0x03, LcdTxBuff, 1);
//  //WriteReg(0x03, 0x1030);

//  //Set GRAM Address
//  LcdTxBuff[0] = ILI9225_HEIGHT;
//  ILI9225_PutCommand(0x20, LcdTxBuff, 1);
//  //WriteReg(0x20, BOARD_LCD_WIDTH);
//  
//  //Set GRAM Address
//  LcdTxBuff[0] = ILI9225_WIDTH;
//  ILI9225_PutCommand(0x21, LcdTxBuff, 1);
//  //WriteReg(0x21, BOARD_LCD_HEIGHT);
////******************

  //LCD_SetRotation(0);

  //LCD Initialization Start  
  //HY1P8TFT_CS_Lo();
  //os_dly_wait(10);
  //vTaskDelay(10);

  //HardWare Reset
  //HY1P8TFT_RESET_Lo();
  //os_dly_wait(30);
  //vTaskDelay(30);
  //HY1P8TFT_RESET_Hi();
  //os_dly_wait(120);
  //vTaskDelay(120);

  //HY1P8TFT_CS_Lo();

//  //Sleep Off
//  HY1P8TFT_PutCommand(0x11, NULL, 0, NULL, 0);
//  //os_dly_wait(120);
//  vTaskDelay(120);

//  //Color Mode
//  LcdTxBuff[0] = 0x05; //16 бит
//	HY1P8TFT_PutCommand(0x3A, LcdTxBuff, 1, NULL, 0);
//  
//  //Frame Rate Control 1
//  LcdTxBuff[0] = 0x00;  //Fastest refresh
//  LcdTxBuff[1] = 0x06;  //6 lines front porch
//  LcdTxBuff[2] = 0x03;  //3 lines back porch
//  HY1P8TFT_PutCommand(0xB1, LcdTxBuff, 3, NULL, 0);

//  //0 Degrees, LtoR, W = 128, H = 160
//  LcdTxBuff[0] = 
//  //LCD_MADCTL_MY | 
//    LCD_MADCTL_MX | 
//  //LCD_MADCTL_MV | 
//  //LCD_MADCTL_ML | 
//  //LCD_MADCTL_RGB | 
//  //LCD_MADCTL_MH |
//    0;
//  LcdTxBuff[0] = LCD_MADCTL_MX;
//  HY1P8TFT_PutCommand(0x36, LcdTxBuff, 1, NULL, 0);

//  //Display On
//  HY1P8TFT_PutCommand(0x29, NULL, 0, NULL, 0);
//  
//  HY1P8TFT_CS_Hi();
//  
//  LCD_SetRotation(90);
  LCD_FillScreen(0);
}

//*****************************************************************************
//
//! The display structure that describes the driver for the Crystalfontz
//! CFAL9664-F-B1 OLED panel with SSD 1332 controller.
//
//*****************************************************************************
//const 
tDisplay g_sILI9225 =
{
    sizeof(tDisplay),
    0,
    ILI9225_HEIGHT,
    ILI9225_WIDTH,
    LCD_PixelDraw,
    LCD_PixelDrawMultiple,
    LCD_LineDrawH,
    LCD_LineDrawV,
    LCD_RectFill,
    LCD_ColorTranslate,
    LCD_Flush
};

