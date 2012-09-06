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

#include "config.h"
#include "pwm.h"
#include "hall.h"
#include "commutate.h"
#include "tinystdio.h"

static unsigned int count = 0;

/*
 * main.c
 */
void main(void)
{
    /* Disable watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* setup clock for 16 MHz internal DCO */
    DCOCTL = 0;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
    BCSCTL2 = 0;
    BCSCTL3 = 0;

    /* drive output latches */
#if INVERT_LOW
    P2OUT = PAL | PBL | PCL;
#else
    P2OUT = 0;
#endif
#if INVERT_HIGH
    P2OUT |= PAH | PBH | PCH;
#endif
    P1OUT = 0;

    /* Enable UART */
    P1SEL = BIT1 | BIT2;
    P1SEL2 = BIT1 | BIT2;

    /* set P2.0 up for PWM */
    P2SEL = 0;
    P2SEL2 = 0;

    /* make all gpio outputs */
    P1DIR = BIT0 | BIT2 | BIT6;
    P2DIR = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
    //P2REN = 0xff;

    /* disable port interrupts */
    P1IES = 0;
    P1IFG = 0;
    P2IES = 0;
    P2IFG = 0;

    pwm1_init(10);

    TA0CCTL2 = CM_0 + CCIS_0 + OUTMOD_6;
    TA0CCTL1 = CM_0 + CCIS_0 + OUTMOD_6;
    TA0CCTL0 = CM_0 + CCIS_0 + OUTMOD_6;
    TA0CCR0 = 1600; /* roughtly 10 kHz */
    TA0CTL = TASSEL_2 + ID_0 + MC_1 + TAIE;

    /* setup UART */
    UCA0CTL0 = 0;
    UCA0CTL1 = UCSSEL_2;
    UCA0BR0 = 131;
    UCA0BR1 = 6;
    UCA0MCTL = UCBRS_1;
    IE2 |= UCA0RXIE;

    printf("hello world\n");

    __enable_interrupt();

    for (;;)
    {
        TA0CTL &= ~TAIE;

        commutate(hall());

        TA0CTL |= TAIE;
    }

}


__interrupt void TIMER0_A1_ISR(void);
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
    TA0CTL &= ~TAIFG;
    count++;
}
