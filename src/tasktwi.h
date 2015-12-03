#ifndef _TASKTWI_H
#define _TASKTWI_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "types.h"
#include "board.h"
#include "tasklcd.h"

//#define LCD_MSG_LM75        (1)
//#define LCD_MSG_BMP180      (2)
//#define LCD_MSG_DHT21       (3)
//#define LCD_MSG_LPH8616     (4)

//typedef struct sLCDMsg
//{
//  U32 Type;
//  union uValue
//  {
//    U32 U32[2];
//    U16 U16[4];
//    U8  U8[8];
//  } Value;
//} tLCDMsg, * pLCDMsg;

void vTWITask(void * pvParameters);

#endif //_TASKTWI_H
