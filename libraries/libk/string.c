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

#include "string.h"
#include <devices/serial/serial.h>

char *string_convert(unsigned int num, int base)
{
    static char Representation[] = "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = Representation[num % base];
        num /= base;
    } while (num != 0);
    return (ptr);
}

void printf(char *format, ...)
{
    unsigned int i;
    unsigned int ZERO = 0;
    char *s;

    va_list arg;
    va_start(arg, format);

    while (*format)
    {

        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
                i = va_arg(arg, int);
                Serial_write(i);
                break;

            case 'd':
                i = va_arg(arg, int);
                if (i < ZERO)
                {
                    i = -i;
                    Serial_write('-');
                }
                Serial_write_string(string_convert(i, 10));
                break;

            case 'o':
                i = va_arg(arg, unsigned int);
                Serial_write_string(string_convert(i, 8));
                break;

            case 's':
                s = va_arg(arg, char *);
                Serial_write_string(s);
                break;

            case 'x':
                i = va_arg(arg, unsigned int);
                Serial_write_string(string_convert(i, 16));
                break;
            default:
                Serial_write('%');
                break;
            }
        }
        else
        {
            Serial_write(*format);
        }
        format++;
    }

    va_end(arg);
}
