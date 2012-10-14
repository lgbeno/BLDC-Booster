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

#include "config.h"
#include "pwm.h"
#include "hall.h"
#include "timer.h"
#include "serial.h";
#include "commutate.h"
#include "tinystdio.h"

void empty_buffer(unsigned int size);

extern unsigned int bemf_chan_lut;
extern unsigned int vpwr_chan_lut;
extern unsigned int state;

uint8_t direction = STARTUP_DIR;

#if DEBUG_VPWR || DEBUG_BEMF || DEBUG_SPEED || DEBUG_INTEGRAL || DEBUG_FILTERED_BEMF
unsigned int debug_buffer[DEBUG_BUFFER_DEPTH];
#endif

uint8_t fill_ptr=1;
uint8_t dump_ptr=0;
uint8_t byte_shift=0;
//                                              (    S1   )   (    S2   )   (    S3   )   (    S4   )   (    S5   )   (    S6   )
static const unsigned int emf_slope_lut [6] = { (   POS   ),  (   NEG   ),  (   POS   ),  (   NEG   ),  (   POS   ),  (   NEG   )};

unsigned int time = 0;
unsigned int speed = 0;
unsigned int integral = 0;
unsigned int threshold = INTEGRAL_THRESH;
uint8_t blank = 2;
unsigned int vpwr = 0;
unsigned int comms = 0;
uint8_t adc_channel = BEMF;
uint8_t adc_last_state = S1;

/*
 * main.c
 */
void main(void)
{
	unsigned int x = 0;
    /* SETUP watchdog */
    WDTCTL = WDTPW + WDTTMSEL + WDTIS1;
    IE1 = WDTIE;

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
#if DEBUG_VPWR || DEBUG_BEMF || DEBUG_SPEED || DEBUG_INTEGRAL || DEBUG_FILTERED_BEMF
    debug_buffer[0] = 0xffff;
#endif

#if SENSORLESS
    //startup sequence
    commutate(S1);
    while (comms < INIT_COMMS)
    {
    	for (x=0;x<1000;x++)
    	{
    		x=x;
    	}
    	commutate_dir(STARTUP_DIR);

    }


    for (;;)
    {
#if DEBUG_VPWR || DEBUG_BEMF || DEBUG_SPEED || DEBUG_INTEGRAL || DEBUG_FILTERED_BEMF
    	empty_buffer(16);
#endif
    }

#else
    for (;;)
	{
    	commutate(hall());

#if DEBUG_VPWR || DEBUG_BEMF || DEBUG_SPEED || DEBUG_INTEGRAL || DEBUG_FILTERED_BEMF
    	empty_buffer(16);
#endif
	}
#endif
}

// Watchdog ISR
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	time++;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	P1OUT ^= BIT6;
	if (adc_last_state != state)
	{
		blank = NUM_BLANKING;
		comms++;
	}

    if (blank==0)
    {
    	unsigned int sample=ADC10MEM  & 0x3FF;

    	if (adc_channel == VPWR)
    	{
    		vpwr = sample;
#if DEBUG_VPWR
    		if (fill_ptr != DEBUG_BUFFER_DEPTH)
    			debug_buffer[fill_ptr++] = sample;
#endif
    	}
    	else
    	{
#if DEBUG_BEMF
			if (fill_ptr != DEBUG_BUFFER_DEPTH)
				debug_buffer[fill_ptr++] = sample;
#endif
    		if (emf_slope_lut [state] ^ direction)
    		{
    			if (sample > (vpwr>>1))
    			{
					if (sample < vpwr)
						sample = vpwr - sample;
					else
						sample = 0;

					integral+=sample;
#if DEBUG_FILTERED_BEMF
					if (fill_ptr != DEBUG_BUFFER_DEPTH)
						debug_buffer[fill_ptr++] = sample;
#endif
    			}
    			else
    			{
    				integral=0;
    			}
    		}
    		else
    		{
    			if (sample < (vpwr>>1))
				{
					integral+=sample;
#if DEBUG_FILTERED_BEMF
					if (fill_ptr != DEBUG_BUFFER_DEPTH)
						debug_buffer[fill_ptr++] = sample;
#endif
				}
    			else
    			{
    				integral=0;
    			}
    		}

#if DEBUG_INTEGRAL
			if (fill_ptr != DEBUG_BUFFER_DEPTH)
				debug_buffer[fill_ptr++] = integral;
#endif
	    	if (integral>threshold)
	    	{
	    		commutate_dir(STARTUP_DIR);
	    		integral = 0;
	    		speed = time;
	    		time=0;
	#if DEBUG_SPEED
			if (fill_ptr != DEBUG_BUFFER_DEPTH)
				debug_buffer[fill_ptr++] = speed;
	#endif
	    	}
    	}
    }
    else
    {
    	blank--;
    }
    ADC10CTL0 &= ~ADC10IFG;
    adc_last_state = state;
	P1OUT ^= BIT6;
}

void empty_buffer(unsigned int size)
{
#if DEBUG_VPWR || DEBUG_BEMF || DEBUG_SPEED || DEBUG_INTEGRAL || DEBUG_FILTERED_BEMF
	if (fill_ptr == DEBUG_BUFFER_DEPTH)
	{
		if (dump_ptr < DEBUG_BUFFER_DEPTH)
		{
			if (IFG2 & UCA0TXIFG)
			{
				byte_shift -= 8;

				UCA0TXBUF = debug_buffer[dump_ptr]>>byte_shift;

				if (byte_shift == 0 )
				{
					dump_ptr++;
					byte_shift = size;
				}
			}
		}
		else
		{
			fill_ptr = 1;
		}
	}
	else
	{
		dump_ptr = 0;
		byte_shift = size;
	}
#endif
}
