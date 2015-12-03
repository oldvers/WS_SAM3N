/* This file Configures the target-dependent low level functions for character I/O */

#include <stdio.h>
#include "SAM3N.h"
#include "core_cm3.h"

/* Disable semihosting */
#pragma import(__use_no_semihosting_swi)

struct __FILE {int handle;};
FILE __stdout;
FILE __stderr;

/*------------------------------------------------------------------------------*/
/*  Outputs a character.                                                        */
/*------------------------------------------------------------------------------*/

// Retargets the C library printf function to the USART

//int fputc(int ch, FILE * f)
//{
//  if((f == stdout) || (f == stderr))
//  {
//    UART_PutChar(ch);
//    return ch ;
//  }
//  else
//  {
//    return EOF;
//  }
//}

// Retargets the C library printf function to the ITM channel 0

int fputc(int ch, FILE * f)
{
  return ITM_SendChar(ch);
}

///*------------------------------------------------------------------------------
// *  Returns the error status accumulated during file I/O.
// *------------------------------------------------------------------------------*/
//int ferror( FILE *f )
//{
//    return EOF ;
//}


void _ttywrch(int ch)
{
  ITM_SendChar(ch); //UART_PutChar( (uint8_t)ch ) ;
}

void _sys_exit(int return_code)
{
  while(1);  //Endless loop
}

/*------------------------------------------------------------------------------
 *  Low level functions I/O for assert().
 *------------------------------------------------------------------------------*/

void __assert_puts(const char *str)
{
    printf("%s", str);
}

/*------------------------------------------------------------------------------
 *  Function for printing info on Hard Fault
 *------------------------------------------------------------------------------*/

enum {r0, r1, r2, r3, r12, lr, pc, psr};

void PrintFaultInfo(uint32_t stack[])
{
  printf("Hard Fault.\r\n");
  printf("  SHCSR    = 0x%.8x\r\n", SCB->SHCSR);
  printf("  CFSR     = 0x%.8x\r\n", SCB->CFSR);
  printf("  HFSR     = 0x%.8x\r\n", SCB->HFSR);
  printf("  MMFAR    = 0x%.8x\r\n", SCB->MMFAR);
  printf("  BFAR     = 0x%.8x\r\n", SCB->BFAR);  

  printf("  R0       = 0x%.8x\r\n", stack[r0]);
  printf("  R1       = 0x%.8x\r\n", stack[r1]);
  printf("  R2       = 0x%.8x\r\n", stack[r2]);
  printf("  R3       = 0x%.8x\r\n", stack[r3]);
  printf("  R12      = 0x%.8x\r\n", stack[r12]);
  printf("  LR [R14] = 0x%.8x - Subroutine call return address\r\n", stack[lr]);
  printf("  PC [R15] = 0x%.8x - Program counter\r\n", stack[pc]);
  printf("  PSR      = 0x%.8x\r\n", stack[psr]);

  while(1);
}
