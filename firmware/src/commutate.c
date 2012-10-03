/** \copyright BSD
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

extern uint32_t integral;
extern unsigned int bemf_adc10ctl1;
extern unsigned int vpwr_adc10ctl1;
extern unsigned int adc_channel;
extern unsigned int state;

/** Advance the commutation state.
 * @param move_to valid values are 1-6
 */
void commutate(unsigned int move_to)
{
    switch (move_to)
    {
        default:
            break;
        case S1:
        	state = 1;
        	bemf_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_4;
        	vpwr_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_5;
            P2SEL = PCL;
#if INVERT_HIGH
            P2OUT |= PBH | PCH;
            P2OUT &= ~PAH;
#else
            P2OUT &= ~(PBH | PCH);
            P2OUT |= PAH;
#endif
            break;
        case S2:
        	state = 2;
        	bemf_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_5;
        	vpwr_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_4;
            P2SEL = PCL;
#if INVERT_HIGH
            P2OUT |= PAH | PCH;
            P2OUT &= ~PBH;
#else
            P2OUT &= ~(PAH | PCH);
            P2OUT |= PBH;
#endif
            break;
        case S3:
        	state = 3;
        	bemf_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_3;
        	vpwr_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_4;
            P2SEL = PAL;
#if INVERT_HIGH
            P2OUT |= PAH | PCH;
            P2OUT &= ~PBH;
#else
            P2OUT &= ~(PAH | PCH);
            P2OUT |= PBH;
#endif
            break;
        case S4:
        	state = 4;
        	bemf_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_4;
        	vpwr_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_3;
            P2SEL = PAL;
#if INVERT_HIGH
            P2OUT |= PAH | PBH;
            P2OUT &= ~PCH;
#else
            P2OUT &= ~(PAH | PBH);
            P2OUT |= PCH;
#endif
            break;
        case S5:
        	state = 5;
        	bemf_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_5;
        	vpwr_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_3;
            P2SEL = PBL;
#if INVERT_HIGH
            P2OUT |= PAH | PBH;
            P2OUT &= ~PCH;
#else
            P2OUT &= ~(PAH | PBH);
            P2OUT |= PCH;
#endif
            break;
        case S6:
        	state = 6;
        	bemf_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_3;
        	vpwr_adc10ctl1 = SHS_0 + CONSEQ_0 + INCH_5;
            P2SEL = PBL;
#if INVERT_HIGH
            P2OUT |= PBH | PCH;
            P2OUT &= ~PAH;
#else
            P2OUT &= ~(PBH | PCH);
            P2OUT |= PAH;
#endif
            break;
    }
}
