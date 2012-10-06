/** \copyright
 * Copyright (c) 2012, Luke Beno, Stuart W. Baker
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
extern unsigned int state;
extern unsigned int adc_channel;


//                                              (    S1   )   (    S2   )   (    S3   )   (    S4   )   (    S5   )   (    S6   )
static const unsigned int bemf_chan_lut [6] = { (  ADC_B ),   (  ADC_A  ),  (  ADC_C  ),  (  ADC_B  ),  (  ADC_A  ),  (  ADC_C  )};
static const unsigned int vpwr_chan_lut [6] = { (  ADC_A ),   (  ADC_B  ),  (  ADC_B  ),  (  ADC_C  ),  (  ADC_C  ),  (  ADC_A  )};

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
#if ADC_MUX_EN
    	ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE; //Use AVCC for REF, 16 clocks, Enable ADC, Interrupt Enable, Disable ADC
#endif
    	ADC10CTL1 = SHS_0 + CONSEQ_0 + bemf_chan_lut[state];
        ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE + ENC; //Use AVCC for REF, 16 clocks, Enable ADC, Interrupt Enable, Enable ADC
    	adc_channel = BEMF;
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
#if ADC_MUX_EN
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE; //Use AVCC for REF, 16 clocks, Enable ADC, Interrupt Enable, Disable ADC
#endif
	ADC10CTL1 = SHS_0 + CONSEQ_0 + vpwr_chan_lut[state];
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE + ENC; //Use AVCC for REF, 16 clocks, Enable ADC, Interrupt Enable, Enable ADC
	adc_channel = VPWR;
    ADC10CTL0 |= ADC10SC;
    TA1CCTL0 &= ~TAIFG;

}


