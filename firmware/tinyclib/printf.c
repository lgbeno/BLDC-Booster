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

#include <stdarg.h>
#include "tinystdio.h"

/** Helper function for converting integers to ascii.
 *
 * @param x
 * @param dp
 */
static void xtoa(unsigned long x, const unsigned long *dp)
{
    char c;
    unsigned long d;
    if(x) {
        while(x < *dp) ++dp;
        do {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            putchar(c);
        } while(!(d & 1));
    } else
        putchar('0');
}

/** Helper function for hexadecimal values.
 *
 * @param n value 0 - 15 to place as a hexadecimal character
 */
static void puth(unsigned n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    putchar(hex[n & 15]);
}

/** Print a formated string to standard out.
 * There are 7 format specifiers:
 * - %c   Character
 * - %s   String
 * - %i   signed Integer (16 bit)
 * - %u   Unsigned integer (16 bit)
 * - %l   signed Long (32 bit)
 * - %n   uNsigned loNg (32 bit)
 * - %x   heXadecimal (16 bit)
 *
 * Field width, floating point and other standard printf() features are not supported.
 *
 * @param format format string
 * @param ... variable arguments list
 * @return always 0 for speed, size, and header file compatibility
 */
int printf(char *format, ...)
{
    char c;
    int i;
    long n;
    static const unsigned long dv[] = {
    //  4294967296      // 32 bit unsigned max
        1000000000,     // +0
         100000000,     // +1
          10000000,     // +2
           1000000,     // +3
            100000,     // +4
    //       65535      // 16 bit unsigned max
             10000,     // +5
              1000,     // +6
               100,     // +7
                10,     // +8
                 1,     // +9
    };

    va_list a;
    va_start(a, format);
    while(c = *format++) {
        if(c == '%') {
            switch(c = *format++) {
                case 's':                       // String
                    puts(va_arg(a, char*));
                    break;
                case 'c':                       // Char
                    putchar(va_arg(a, char));
                    break;
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, putchar('-');
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit unsigned long
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, putchar('-');
                    xtoa((unsigned long)n, dv);
                    break;
                case 'x':                       // 16 bit hexadecimal
                    i = va_arg(a, int);
                    puth(i >> 12);
                    puth(i >> 8);
                    puth(i >> 4);
                    puth(i);
                    break;
                case 0: return 0;
                default: goto bad_fmt;
            }
        } else
bad_fmt:    putchar(c);
    }
    va_end(a);

    return 0;
}
