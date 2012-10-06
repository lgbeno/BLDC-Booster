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

/** Retrieve the hall effect sensor value.
 *
 * @return valid values are between 1 and 6
 */
unsigned int hall(void)
{
    uint8_t hall;
    uint8_t ret;

    hall =  (HALL_A_PORT & HALL_A_MASK) >> HALL_A_SHIFT;
    hall += (HALL_B_PORT & HALL_B_MASK) >> HALL_B_SHIFT;
    hall += (HALL_C_PORT & HALL_C_MASK) >> HALL_C_SHIFT;

    switch (hall)
    {
    	case 1:
    		ret = S4;
    		break;
    	case 2:
    		ret = S6;
    		break;
    	case 3:
    		ret = S5;
    		break;
    	case 4:
    		ret = S2;
    		break;
    	case 5:
    		ret = S3;
    		break;
    	case 6:
    		ret = S1;
    		break;
    	default:
    		ret = S1;
    		break;
    }
    return ret;
}
