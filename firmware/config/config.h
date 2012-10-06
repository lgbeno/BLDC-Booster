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

#include <msp430.h>


#define CCW 0
#define CW 1

#define SENSORLESS 1
#define NUM_BLANKING 4
#define INTEGRAL_THRESH 1850
#define DUTY_STARTUP 50
#define STARTUP_DIR CW

#define ADC_MUX_EN 1
#define DEFAULT_ADC_CHANNEL INCH_5
#define DEBUG_BUFFER_DEPTH 80
#define DEBUG_INTEGRAL 0
#define DEBUG_VPWR 0
#define DEBUG_BEMF 0

#define INIT_COMMS 100

#define BEMF 0
#define VPWR 1

#define INVERT_HIGH 0
#define INVERT_LOW  1

#define PAL BIT1
#define PAH BIT0
#define PBL BIT2
#define PBH BIT3
#define PCL BIT4
#define PCH BIT5

#define POS 0
#define NEG 1

#define ADC_A INCH_5
#define ADC_B INCH_4
#define ADC_C INCH_3

#define HALL_A_PORT  P2IN
#define HALL_B_PORT  P2IN
#define HALL_C_PORT  P1IN
#define HALL_A_MASK  0x40
#define HALL_B_MASK  0x80
#define HALL_C_MASK  0x80
#define HALL_A_SHIFT 6
#define HALL_B_SHIFT 6
#define HALL_C_SHIFT 5

#define S1      0
#define S2      1
#define S3      2
#define S4      3
#define S5      4
#define S6      5
