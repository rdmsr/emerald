/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2020 Abb1x
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef STRING_H
#define STRING_H
#include <stdarg.h>
#include <stddef.h>

/**
@file string.h
A header used for string manipulation & printing.
*/

/** The standard printf function, prints using serial. */
void printf(char *format, ...);

/** Converts a number to a string.
@param[in] num The actual number.
@param[in] base The base used in the number (e.g: 16,10,...).
@return The number turned to string.
*/
char *string_convert(unsigned int num, int base);

/** Converts int to string (function will be removed)*/
int atoi(char *str);

/** Gets the length of a string
@param[in] str The string to get the length of.
@return The size of the string
*/
size_t strlen(char *str);

/** Concatenates two strings up to a specified length.
@param[in] dest The original string.
@param[in] src  The string to be appended.
@param[in] n    The maximum number of character to be appended.
@return The concatenated string.
*/
char *strncat(char *dest, char *src, size_t n);

/** Concatenates two strings.
@param[in] dest The original string.
@param[in] src The string to be appended.
@return The concatenated string.
*/
char *strcat(char *dest, char *src);

/** Formats a string to a buffer.
@param[out] str The buffer.
@param[in] format The format string.
@param[in] arg The list of optional arguments.
*/
void vsprintf(char *str, char *format, va_list arg);
#endif
