/** \copyright
 * Copyright (c) 2012, Stuart W. Baker
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "serial.h"

char rxBuf[SERIAL_BUF_SIZE];
char txBuf[SERIAL_BUF_SIZE];
int txIndexWr = 0;
int txIndexRd = 0;
int rxIndexWr = 0;
int rxIndexRd = 0;

/** Initialize serial driver.
 */
void serial_init(void)
{
    /* setup UART */
    UCA0CTL0 = 0;
    UCA0CTL1 = UCSSEL_2;
    UCA0BR0 = 131;
    UCA0BR1 = 6;
    UCA0MCTL = UCBRS_1;
    IE2 |= UCA0RXIE;
}

__interrupt void USCIAB0RX_ISR(void);
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
    while (!(IFG2 & UCA0TXIFG))
    {
        rxBuf[rxIndexWr++] = UCA0RXBUF;
        if (rxIndexWr >= SERIAL_BUF_SIZE)
        {
            rxIndexWr = 0;
        }
    }
}

__interrupt void USCIAB0TX_ISR(void);
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    UCA0TXBUF = txBuf[txIndexRd++];
    if (txIndexRd >= SERIAL_BUF_SIZE)
    {
        txIndexRd = 0;
    }
    if (txIndexRd == txIndexWr)
    {
        IE2 &= ~UCA0TXIE;
    }
}

