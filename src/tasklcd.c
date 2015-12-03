#include "tasklcd.h"


void SPI_IRQHandler(void)
{
  SPIM_Handler();
}


void vLCDTask(void * pvParameters)
{
  tContext       g_sContext;
  tRectangle     sRect;
  tLCDMsg        Msg;
  char           StrBuf[32];
  QueueHandle_t  LCDMsgQueue = (QueueHandle_t)pvParameters;
  
  //LCD Initialization  
  LCD_Init();
  LCD_SetBacklight(5);
  LCD_On();
  LCD_SetRotation(LCD_ORIENT_270);
  
  //Initialize the graphics context and find the middle X coordinate.
  g_sILI9225.ui16Height = LCD_GetHeight();
  g_sILI9225.ui16Width  = LCD_GetWidth();
  GrContextInit(&g_sContext, &g_sILI9225);

  //Set Font
  GrContextFontSet(&g_sContext, g_psFontBookmanold20b);
  
  //Temperature Frame
  sRect.i16XMin = 0;
  sRect.i16YMin = 0;
  sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
  sRect.i16YMax = 23;
  GrContextForegroundSet(&g_sContext, ClrYellow);
  GrRectDraw(&g_sContext, &sRect);
  sRect.i16XMin = 1;
  sRect.i16YMin = 1;
  sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 2;
  sRect.i16YMax = 22;
  GrContextForegroundSet(&g_sContext, ClrBlue);
  GrRectFill(&g_sContext, &sRect);
  
  sprintf(StrBuf, "Temperature");
  GrContextForegroundSet(&g_sContext, ClrWhite);
  GrStringDraw(&g_sContext, StrBuf, -1, 5, 2, 0);
  
  
//  sRect.i16YMin = 20;
//  sRect.i16YMax = 39;
//  GrContextForegroundSet(&g_sContext, ClrBlue);
//  GrRectFill(&g_sContext, &sRect);
//  sprintf(StrBuf, "P = %.1f C", 34.8);
//  GrContextForegroundSet(&g_sContext, ClrWhite);
//  GrStringDraw(&g_sContext, StrBuf, -1, 2, 22, 0);



  for(;;)
  {
    xQueueReceive(LCDMsgQueue, (void *)&Msg, portMAX_DELAY);
    
    switch(Msg.Type)
    {
      case LCD_MSG_LM75:  /* LM75 Temperature */
        sRect.i16XMin = 0;
        sRect.i16YMin = 24;
        sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
        sRect.i16YMax = 47;
        GrContextForegroundSet(&g_sContext, ClrBlack);
        GrRectFill(&g_sContext, &sRect);
        sprintf(StrBuf, "T = %.1f C", Msg.Value.U16[0] * 0.5);
        GrContextForegroundSet(&g_sContext, ClrWhite);
        GrStringDraw(&g_sContext, StrBuf, -1, 8, 26, 0);
        break;
      case LCD_MSG_BMP180:  /* BM180 Temperature & Pressure */
//        sRect.i16XMin = 0;
//        sRect.i16YMin = 19;
//        sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
//        sRect.i16YMax = 76;
//        GrContextForegroundSet(&g_sContext, ClrRed);
//        GrRectFill(&g_sContext, &sRect);

//        sprintf(StrBuf, "T = %.1f C", Msg.Value.U16[0] * 0.1);
//        GrContextForegroundSet(&g_sContext, ClrWhite);
//        GrStringDraw(&g_sContext, StrBuf, -1, 2, 19, 0);

//        sprintf(StrBuf, "P = %d Pa", Msg.Value.U32[1]);
//        GrContextForegroundSet(&g_sContext, ClrWhite);
//        GrStringDraw(&g_sContext, StrBuf, -1, 2, 38, 0);
//      
//        sprintf(StrBuf, "P = %.1f mm", Msg.Value.U32[1] / 133.322);
//        GrContextForegroundSet(&g_sContext, ClrWhite);
//        GrStringDraw(&g_sContext, StrBuf, -1, 2, 57, 0);
        break;
      case LCD_MSG_DHT21:  /* DHT21 Temperature & Humidity */
//        sRect.i16XMin = 0;
//        sRect.i16YMin = 76;
//        sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
//        sRect.i16YMax = 114;
//        GrContextForegroundSet(&g_sContext, ClrYellow);
//        GrRectFill(&g_sContext, &sRect);

//        sprintf(StrBuf, "T = %.1f C", Msg.Value.U16[0] * 0.1);
//        GrContextForegroundSet(&g_sContext, ClrBlue);
//        GrStringDraw(&g_sContext, StrBuf, -1, 2, 76, 0);

//        sprintf(StrBuf, "H = %.1f %%", Msg.Value.U16[1] * 0.1);
//        GrContextForegroundSet(&g_sContext, ClrBlue);
//        GrStringDraw(&g_sContext, StrBuf, -1, 2, 95, 0);
        break;
      case LCD_MSG_LPH8616:  /* LPH8616 LCD */
//        sRect.i16XMin = 0;
//        sRect.i16YMin = 0;
//        sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
//        sRect.i16YMax = 19;
//        GrContextForegroundSet(&g_sContext, ClrBlue);
//        GrRectFill(&g_sContext, &sRect);
//        sprintf(StrBuf, "LCD P = %02x", Msg.Value.U8[0]);
//        GrContextForegroundSet(&g_sContext, ClrWhite);
//        GrStringDraw(&g_sContext, StrBuf, -1, 2, 0, 0);
        break;
      default:
        break;
    }
  }
}
