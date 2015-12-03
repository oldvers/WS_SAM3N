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
 * Implementation of SPI master wrapper layer.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "board.h"

/*----------------------------------------------------------------------------
 *        Internal structures
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/** Semaphore used as a semaphore event */
static volatile uint8_t eotEvent = 0;

extern ESpimStatus SPIM_InitializeContext( Spi* pSpi, IRQn_Type spiId )
{
    pSpi = pSpi; /* stop warning */
    
    /* Configure the interrupt with the according priority */
    NVIC_SetPriority( spiId, 1 ) ;

    return SPIM_OK;
}

extern ESpimStatus SPIM_LockSemaphore( Spi* pSpi )
{
    pSpi = pSpi; /* stop warning */
    return SPIM_OK;
}

extern void SPIM_ReleaseSemaphore( Spi* pSpi )
{
    pSpi = pSpi; /* stop warning */
}


extern ESpimStatus SPIM_CreateEotEvent( Spi* pSpi )
{
    pSpi = pSpi; /* stop warning */
    eotEvent = 0;
    return SPIM_OK;
}

extern void SPIM_SignalEotEvent( Spi* pSpi )
{
    pSpi = pSpi; /* stop warning */
    eotEvent = 1;
}

extern ESpimStatus SPIM_WaitEotEvent( Spi* pSpi )
{
    pSpi = pSpi; /* stop warning */
    while (eotEvent == 0);
    return SPIM_OK;
}
