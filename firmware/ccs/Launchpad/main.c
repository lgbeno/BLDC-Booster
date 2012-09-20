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
#include "timer.h"
#include "serial.h"
#include "commutate.h"
#include "tinystdio.h"

uint32_t integral = 0;
unsigned int blank = 2;


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

    pwm1_init(70);
    timera0_init();
    serial_init();
    ADC10CTL1 = SHS_0 + CONSEQ_0 + INCH_3; //+ ADC10DIV2 + ADC10DIV1 + ADC10DIV0;
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE; //Use AVCC for REF, 16 clocks, Enable ADC, Interrupt Enable
    ADC10CTL0 |= ENC;                         // ADC10 Enable
    ADC10AE0 |= 0x38;                         // P1.3 ADC10 option select*/

    printf("hello world\n");

    __enable_interrupt();

#if SENSORLESS
    unsigned int hall_last = hall();
#if 0
    int i;
    for (i = 0; i < 20; )
    {

        unsigned int halla = hall();
        commutate(halla);

        if (halla != hall_last && (hall_last == S3 || hall_last == S6))
        {
            i++;
            integral = 0;
        }
        hall_last = halla;
    }
#endif
    unsigned int state = hall_last;
    for (;;)
    {
        if (integral >= 130000)
        {
            switch (state)
            {
                case S1:
                    commutate(S6);
                    state = S6;
                    break;
                case S2:
                    commutate(S1);
                    state = S1;
                    break;
                case S3:
                    commutate(S2);
                    state = S2;
                    break;
                case S4:
                    commutate(S3);
                    state = S3;
                    break;
                case S5:
                    commutate(S4);
                    state = S4;
                    break;
                case S6:
                    commutate(S5);
                    state = S5;
                    break;
            }
            integral = 0;
        }
    }
#endif

#if !SENSORLESS
    unsigned int state = S2;
    unsigned int last_state = S1;
    uint32_t integral_temp = 0;
    commutate(last_state);

    for (;;)
        {
    		state=hall();
    		if (last_state != state)
    	    {
    	    	commutate(state);
    	    	integral_temp=integral;
    	    	integral=0;
    	    	blank=1;

				#if INTEGRAL_DEBUG
    	    	putchar(integral_temp>>24);
    	    	putchar(integral_temp>>16);
    	    	putchar(integral_temp>>8);
    	    	putchar(integral_temp);
				#endif
    	    }
	    	last_state=state;
        }
#endif
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	P1OUT ^=BIT0;
    if (blank<=0)
    {
    	unsigned int sample=ADC10MEM;
    	integral += sample;

		#if ADC_SAMPLE_DEBUG
    	putchar(sample>>8);
    	putchar(sample);
		#endif
    }
    else
    {
    	blank--;
    }
    ADC10CTL0 &= ~ADC10IFG;
}
