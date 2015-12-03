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

/*
 * This file Configures the target-dependent low level functions for character I/O.
 */

#include "board.h"
#include <stdio.h>

/* Disable semihosting */
#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle;} ;
FILE __stdout;
FILE __stderr;

/*------------------------------------------------------------------------------
 *  Outputs a character.
 *------------------------------------------------------------------------------*/
int fputc(int ch, FILE *f)
{
    if ((f == stdout) || (f == stderr))
    {
        UART_PutChar( ch ) ;
        return ch ;
    }
    else
    {
        return EOF ;
    }
}

/*------------------------------------------------------------------------------
 *  Returns the error status accumulated during file I/O.
 *------------------------------------------------------------------------------*/
int ferror( FILE *f )
{
    return EOF ;
}


void _ttywrch( int ch )
{
    UART_PutChar( (uint8_t)ch ) ;
}

void _sys_exit(int return_code)
{
    while ( 1 ) ;  /* endless loop */
}

/*------------------------------------------------------------------------------
 *  Low level functions I/O for assert().
 *------------------------------------------------------------------------------*/
void __assert_puts(const char *str)
{
    printf("%s", str);
}

