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
#include "serial.h";
#include "commutate.h"
#include "tinystdio.h"

uint32_t integral = 0;
uint32_t integral_thresh = 0;
uint32_t integral_spread = 0;
unsigned int blank = 2;
unsigned int bemf_adc10ctl1;
unsigned int vpwr_adc10ctl1;
unsigned int adc_channel;
unsigned int vpwr;
unsigned int center;
unsigned int state;



/* Debug Global Declarations*/
#if ADC_SAMPLE_DEBUG || INTEGRAL_DEBUG
unsigned int debug_ref=0;
unsigned int fill=1;
#endif
#if ADC_SAMPLE_DEBUG
unsigned int debug_buff[DEBUG_BUFFER_DEPTH];
#endif
#if INTEGRAL_DEBUG
uint32_t debug_buff[DEBUG_BUFFER_DEPTH];
unsigned int sub_ref=0;
#endif

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

    /*initialize the peripherals*/
    pwm1_init(DUTY_STARTUP);
    timera0_init();
    serial_init();

    /*setup the ADC*/
    ADC10CTL1 = SHS_0 + CONSEQ_0 + DEFAULT_ADC_CHANNEL; //+ ADC10DIV2 + ADC10DIV1 + ADC10DIV0;
    ADC10CTL0=0;
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE + ENC; //Use AVCC for REF, 16 clocks, Enable ADC, Interrupt Enable, Enable ADC
    ADC10AE0 |= 0x38;                         // P1.3 ADC10 option select*/

    __enable_interrupt();

#if SENSORLESS
    unsigned int stat = S2;
    unsigned int last_state = S1;
    unsigned int comms = 0;

    //startup sequence
    commutate(last_state);
    while (comms < INIT_COMMS)
    {
    	state=hall();
    	if (state!=last_state)
    	{
    		commutate(state);
    		comms++;
    	}
    	last_state=state;
    }

    for (;;)
    {
    	if (integral >= INTEGRAL_THRESH)
    	{
 			P1OUT ^=BIT6;
    		switch(state)
    		{
    			case S1:
    				commutate(S2);
    				break;
    			case S2:
    				commutate(S3);
    				break;
    			case S3:
    				commutate(S4);
    				break;
    			case S4:
    				commutate(S5);
    				break;
    			case S5:
    				commutate(S6);
    				break;
    			case S6:
    				commutate(S1);
    				break;
    		}
 			integral = 0;
    	}

    }

#else
    unsigned int state = S2;
    unsigned int last_state = S1;
    commutate(last_state);

    for (;;)
        {
     		if (integral >= INTEGRAL_THRESH)
        	{
     			P1OUT ^=BIT6;
     			integral = 0;
        	}

    		state=hall();
    		if (last_state != state)
    	    {
    	    	commutate(state);
#if INTEGRAL_DEBUG
    			if (fill==1)
    			{
    				if (debug_ref<DEBUG_BUFFER_DEPTH)
    				{
    					debug_buff[debug_ref]=integral;
    					debug_ref++;
    				}
    				else
    				{
    					putchar(0xFF);
    					putchar(0xFF);
    					putchar(0xFF);
    					putchar(0xFF);
    					debug_ref=0;
    					fill=0;
    				}
    			}
#endif
    	    	//integral=0;
    	    	blank=NUM_BLANKING;
    	    }
	    	last_state=state;
        }
#endif
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    if (blank<=0)
    {
    	unsigned int sample=ADC10MEM;
    	if (adc_channel == VPWR)
    	{
    		center = sample/2;
    		vpwr = sample;
    	}
    	else
    	{
    		if (state % 2 == 1)
    		{
    			sample = vpwr - sample;
    		}
			if (sample > center)
			{
				P1OUT ^=BIT0;
				integral+=sample;
			}
    	}
#if STREAM_ADC_SAMPLES
		putchar(sample>>8);
		putchar(sample);
#endif
#if ADC_SAMPLE_DEBUG
		if (fill==1)
		{
			if (debug_ref<DEBUG_BUFFER_DEPTH)
			{
				debug_buff[debug_ref]=sample;
				debug_ref++;
			}
			else
			{
				putchar(0xFF);
				putchar(0xFF);
				debug_ref=0;
				fill=0;
			}
		}
#endif
    }
    else
    {
#if STREAM_ADC_SAMPLES
		putchar(0xFF);
		putchar(0xFF);
#endif
    	blank--;
    }

#if ADC_SAMPLE_DEBUG || INTEGRAL_DEBUG
    if (fill==0)
    {
    	if (debug_ref<DEBUG_BUFFER_DEPTH)
    	{
#if INTEGRAL_DEBUG
    		if (sub_ref==0)
    		{
    			putchar(debug_buff[debug_ref]>>24);
    			putchar(debug_buff[debug_ref]>>16);
    			sub_ref=1;
    		}
    		else
    		{
    			putchar(debug_buff[debug_ref]>>8);
    			putchar(debug_buff[debug_ref]);
    			sub_ref=0;
    			debug_ref++;
    		}
#endif
#if ADC_SAMPLE_DEBUG
			putchar(debug_buff[debug_ref]>>8);
			putchar(debug_buff[debug_ref]);
			debug_ref++;
#endif
    	}
    	else
    	{
			debug_ref=0;
			fill=1;
    	}
    }
#endif

    ADC10CTL0 &= ~ADC10IFG;
}
