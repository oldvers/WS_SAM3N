#include "FreeRTOS.h"
#include "task.h"

#include "chip.h"
#include "board.h"
#include "tasklcd.h"
#include "tasktwi.h"


void vLEDaTask(void * pvParameters)
{
  LED_Configure(0);
  LED_Configure(1);
  LED_Configure(2);
  LED_Set(1);
  
  for(;;)
  {
    LED_Toggle(0);
    LED_Toggle(1);
    LED_Toggle(2);
    vTaskDelay(500);
  }
}










int main(void)
{
  QueueHandle_t LCDMsgQueue;
  tLCDMsg Msg;

  LowLevelInit();
  
  printf("All OK!\r\n");
  
  /* Disable watchdog */
  WDT_Disable(WDT);
  
  LCDMsgQueue = xQueueCreate(10, sizeof(tLCDMsg));
  //LM75 Rect
  Msg.Type = LCD_MSG_LM75;
  Msg.Value.U8[0] = 25;
  xQueueSend(LCDMsgQueue, (void *)&Msg, 0);
  //BMP180 Rect
  Msg.Type = LCD_MSG_BMP180;
  Msg.Value.U16[0] = 250;
  Msg.Value.U16[1] = 10150;
  xQueueSend(LCDMsgQueue, (void *)&Msg, 0);
  //DHT21 Rect
  Msg.Type = LCD_MSG_DHT21;
  Msg.Value.U16[0] = 250;
  Msg.Value.U16[1] = 650;
  xQueueSend(LCDMsgQueue, (void *)&Msg, 0);
  
  
//  QueueHandle_t LCDMsgQueue;
//  tLCDMsg Msg;
//  
//  LCDMsgQueue = xQueueCreate(10, sizeof(tLCDMsg));
//  //LM75 Rect
//  Msg.Type = LCD_MSG_LM75;
//  Msg.Value.U8[0] = 25;
//  xQueueSend(LCDMsgQueue, (void *)&Msg, 0);
//  //BMP180 Rect
//  Msg.Type = LCD_MSG_BMP180;
//  Msg.Value.U16[0] = 250;
//  Msg.Value.U16[1] = 10150;
//  xQueueSend(LCDMsgQueue, (void *)&Msg, 0);
//  //DHT21 Rect
//  Msg.Type = LCD_MSG_DHT21;
//  Msg.Value.U16[0] = 250;
//  Msg.Value.U16[1] = 650;
//  xQueueSend(LCDMsgQueue, (void *)&Msg, 0);
  
  xTaskCreate(vLEDaTask,"LEDaTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(vLCDTask,"LCDTask", configMINIMAL_STACK_SIZE, (void *)LCDMsgQueue, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(vTWITask,"TWITask", configMINIMAL_STACK_SIZE, (void *)LCDMsgQueue, tskIDLE_PRIORITY + 1, NULL);
//  xTaskCreate(vLPHTask,"LPHTask", configMINIMAL_STACK_SIZE, (void *)LCDMsgQueue, tskIDLE_PRIORITY + 1, NULL);
//  xTaskCreate(vPressTask, "PressTask", configMINIMAL_STACK_SIZE, (void *)LCDMsgQueue, tskIDLE_PRIORITY + 1, NULL);
//  xTaskCreate(vHumidTask, "HumidTask", configMINIMAL_STACK_SIZE, (void *)LCDMsgQueue, tskIDLE_PRIORITY + 1, NULL);
//  xTaskCreate(vLEDSTask,"LEDSTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

  vTaskStartScheduler();
  
//  //***********************************************************************************
//  // Test Sequence
//  //***********************************************************************************
//  
//  U8 i2cbuf[10], Adr;
//  U32 Delay = 3000000;
//  
//  LEDs_Init();
//  
//  //DBGMCU->CR |= DBGMCU_TIM3_STOP;
//  DBGMCU->CR |= DBGMCU_I2C1_SMBUS_TIMEOUT;
//  
//  I2C_LowLevel_Init(I2C1);
//  
//  while(1)
//  {
//    i2cbuf[0] = 0x00;
//    
//    for(Adr = 0; Adr < 255; Adr++)
//    {
//    //Adr = 0x48;
//      if(I2C_Master_BufferWrite(I2C1, i2cbuf, 1, Polling, Adr) == Success)
//      {       
//        if(I2C_Master_BufferRead(I2C1, i2cbuf, 2, Polling, Adr) == Success)
//        {
//          LED_G_On();
//          Delay = 300000;
//          while(Delay) Delay--;
//        }
//      }
//      else
//      {
//        I2C1->CR1 |= CR1_STOP_Set;
//        /* Make sure that the STOP bit is cleared by Hardware */
//        while ((I2C1->CR1&0x200) == 0x200);
//        LED_G_Off();
//      }
//    }
//    
//    Delay = 300000;
//    while(Delay) Delay--;
//  }
//  
//  //***********************************************************************************
//  //
//  //***********************************************************************************

  while(1);
}
