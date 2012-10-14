/** \copyright BSD
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


unsigned int state = 0;

extern uint32_t integral;

#if INVERT_HIGH
	//                                              (    S1   )   (    S2   )   (    S3   )   (    S4   )   (    S5   )   (    S6   )
	static const unsigned int p2out_set_lut [6] = { (PBH | PCH),  (PAH | PCH),  (PAH | PCH),  (PAH | PBH),  (PAH | PBH),  (PBH | PCH)};
	static const unsigned int p2out_clr_lut [6] = { (  ~PAH   ),  (  ~PBH   ),  (  ~PBH   ),  (  ~PCH   ),  (  ~PCH   ),  (  ~PAH   )};
	static const unsigned int p2sel_set_lut [6] = { (   PCL   ),  (   PCL   ),  (   PAL   ),  (   PAL   ),  (   PBL   ),  (   PBL   )};
//	static const unsigned int bemf_chan_lut [6] = { (  ADC_B  ),  (  ADC_A  ),  (  ADC_C  ),  (  ADC_B  ),  (  ADC_A  ),  (  ADC_C  )};
//	static const unsigned int vpwr_chan_lut [6] = { (  ADC_A  ),  (  ADC_B  ),  (  ADC_B  ),  (  ADC_C  ),  (  ADC_C  ),  (  ADC_A  )};
#else
	//                                              (    S1   )   (    S2   )   (    S3   )   (    S4   )   (    S5   )   (    S6   )
	static const unsigned int p2out_set_lut [6] = { (   PAH   ),  (    PBH  ),  (   PBH   ),  (   PCH   ),  (   PCH   ),  (   PAH   )};
	static const unsigned int p2out_clr_lut [6] = {~(PBH | PCH), ~(PAH | PCH), ~(PAH | PCH), ~(PAH | PBH), ~(PAH | PBH), ~(PBH | PCH)};
	static const unsigned int p2sel_set_lut [6] = { (  PCL   ),   (   PCL   ),  (   PAL   ),  (   PAL   ),  (   PBL   ),  (   PBL   )};
//	static const unsigned int bemf_chan_lut [6] = { (  ADC_B ),   (  ADC_A  ),  (  ADC_C  ),  (  ADC_B  ),  (  ADC_A  ),  (  ADC_C  )};
//	static const unsigned int vpwr_chan_lut [6] = { (  ADC_A ),   (  ADC_B  ),  (  ADC_B  ),  (  ADC_C  ),  (  ADC_C  ),  (  ADC_A  )};
#endif

	void commutate(unsigned int move_to)
	{
    	P1OUT ^= BIT0;
		P2OUT |= p2out_set_lut[state];
		P2OUT &= p2out_clr_lut[state];
		P2SEL  = p2sel_set_lut[state];
		state = move_to;
	}

/** Advance the commutation state.
 * @param direction valid values are 0-1
 */
void commutate_dir(unsigned int direction)
{
	if (direction==CCW)
	{
		if (state != S6)
			state++;
		else
			state = S1;
	}
	else
	{
		if (state != S1)
			state--;
		else
			state = S6;
	}
	commutate(state);
}
