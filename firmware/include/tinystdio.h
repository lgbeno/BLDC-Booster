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

#ifndef _tinystdio_h_
#define _tinystdio_h_

#ifndef EOF
#define EOF -1
#endif

#ifndef stdin
#define stdin (void*)0
#endif
#ifndef stdout
#define stdout (void*)1
#endif
#ifndef stderr
#define stderr (void*)2
#endif

#ifndef _STDIO
#define _STDIO
typedef void *FILE;
#endif

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
int printf(char *format, ...);

/** Write a character to standard out.
 *
 * @param c character to write
 * @return character written if successful, else EOF and errno set
 */
int putchar(int c);

/** Write a string to standard out.
 *
 * @param s string to write
 * @return positive number upon success, else EOF with errno set
 */
int puts(char *s);

/** Get the next character from standard in.
 *
 * @return character if successful, else EOF on error with errno set
 */
int getchar(void);

/** Read a character from a stream.  Note, only standard in is supported.
 *
 * @param s string to read into
 * @param n n - 1 = number of bytes to read
 * @param stream ignored, will always read from standard in
 * @return pointer to string on success, NULL on failure
 */
char *fgets(char *s, int n, FILE *stream);

#endif /* _tinystdio_h_ */
