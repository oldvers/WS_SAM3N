#ifndef _TASKLCD_H
#define _TASKLCD_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "types.h"
#include "lcd.h"

#define LCD_MSG_LM75        (1)
#define LCD_MSG_BMP180      (2)
#define LCD_MSG_DHT21       (3)
#define LCD_MSG_LPH8616     (4)

typedef struct sLCDMsg
{
  U32 Type;
  union uValue
  {
    U32 U32[2];
    U16 U16[4];
    U8  U8[8];
  } Value;
} tLCDMsg, * pLCDMsg;

void vLCDTask(void * pvParameters);

#endif //_TASKLCD_H
