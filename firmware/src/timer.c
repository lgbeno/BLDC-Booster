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

#include <stdint.h>
#include "config.h"

/** Free running timer count */
static uint32_t count = 0;
static char which = 0;
extern uint32_t integral;

/** Initialize the free running timer based off of timer A0.
 */
void timera0_init(void)
{
    TA0CCTL2 = CM_0 + CCIS_0 + OUTMOD_6;
    TA0CCTL1 = CM_0 + CCIS_0 + OUTMOD_6;
    TA0CCTL0 = CM_0 + CCIS_0 + OUTMOD_6;
    TA0CCR0 = 1599; /* roughtly 10 kHz */
    TA0CTL = TASSEL_2 + ID_0 + MC_1 + TAIE;
    which = 1;
}

/** Initialize the free running timer based off of timer A1.
 */
void timera1_init(void)
{
    TA1CCTL2 = CM_0 + CCIS_0 + OUTMOD_6;
    TA1CCTL1 = CM_0 + CCIS_0 + OUTMOD_6;
    TA1CCTL0 = CM_0 + CCIS_0 + OUTMOD_6;
    TA1CCR0 = 1599; /* roughtly 10 kHz */
    TA1CTL = TASSEL_2 + ID_0 + MC_1;
    which = 2;
}

/** Get the current time.
 *
 * @return current time in microseconds since startup
 */
uint32_t get_time(void)
{
    uint32_t result;

    switch (which)
    {
        default:
            result = 0;
            break;
        case 1:
            TA0CTL &= ~TAIE;
            result = count;
            TA0CTL |= TAIE;
            break;
        case 2:
            TA1CTL &= ~TAIE;
            result = count;
            TA1CTL |= TAIE;
            break;
    }

    return result;
}

/** Timer 0 A1 interrupt.
 */
__interrupt void TIMER0_A1_ISR(void);
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
    TA0CTL &= ~TAIFG;

    count += 100;
}

/** Timer 1 A1 interrupt.
 */
__interrupt void TIMER1_A1_ISR(void);
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
    if (TA1IV == 0x0A)
    {
        ADC10CTL0 |= ADC10SC;
        TA1CCTL0 &= ~TAIFG;
    }

    TA1CCTL0 &= ~CCIFG;
}

/** Timer A1 interrupt.
 */
__interrupt void TIMER1_A0_ISR(void);
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
}


