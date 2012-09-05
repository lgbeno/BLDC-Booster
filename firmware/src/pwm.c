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

/** Initialize PWM channel 1.
 *
 * @param percent_duty_cycle initial duty cycle, 0 - 100
 */
void pwm1_init(unsigned int percent_duty_cycle)
{
    /* TA1CCTL1, Capture/Compare Control Register 1
     * Configure into PWM toggle/set mode
     */
    TA1CCTL2 = CM_0 + CCIS_0 + OUTMOD_6;
    TA1CCTL1 = CM_0 + CCIS_0 + OUTMOD_6;
    TA1CCTL0 = CM_0 + CCIS_0 + OUTMOD_6;

    TA1CCR0 = 800; /* roughly 10 kHz */
    pwm1_duty_cycle(percent_duty_cycle);

    /* TA1CTL, Timer_A3 Control Register
     * SMCLK source, divide by 1, up/down mode
     */
    TA1CTL = TASSEL_2 + ID_0 + MC_3;
}

/** Update PWM channel 1 duty cycle.
 *
 * @param percent initial duty cycle, 0 - 100
 */
void pwm1_duty_cycle(unsigned int percent)
{
    if (percent > 100)
    {
        return;
    }

    percent *= 8;
#if INVERT_LOW
    TA1CCR1 = percent;
    TA1CCR2 = percent;
#else
    TA1CCR1 = 800 - percent;
    TA1CCR2 = 800 - percent;
#endif
}

/** Initialize PWM channel 0.
 *
 * @param percent_duty_cycle initial duty cycle, 0 - 100
 */
void pwm0_init(unsigned int percent_duty_cycle)
{
    /* TA1CCTL1, Capture/Compare Control Register 1
     * Configure into PWM toggle/set mode
     */
    TA0CCTL2 = CM_0 + CCIS_0 + OUTMOD_6;
    TA0CCTL1 = CM_0 + CCIS_0 + OUTMOD_6;
    TA0CCTL0 = CM_0 + CCIS_0 + OUTMOD_6;

    TA0CCR0 = 800; /* roughly 10 kHz */
    pwm0_duty_cycle(percent_duty_cycle);

    /* TA1CTL, Timer_A3 Control Register
     * SMCLK source, divide by 1, up/down mode
     */
    TA0CTL = TASSEL_2 + ID_0 + MC_3;
}

/** Update PWM channel 0 duty cycle.
 *
 * @param percent initial duty cycle, 0 - 100
 */
void pwm0_duty_cycle(unsigned int percent)
{
    if (percent > 100)
    {
        return;
    }

    percent *= 8;
#if INVERT_LOW
    TA0CCR1 = percent;
    TA0CCR2 = percent;
#else
    TA0CCR1 = 800 - percent;
    TA0CCR2 = 800 - percent;
#endif
}
