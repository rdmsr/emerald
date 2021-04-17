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

    va_list arg;
    va_start(arg, format);


    char message[4096] = {0};
    vsprintf(message, format, arg);
    Serial_write_string(message);
    va_end(arg);
}

void vsprintf(char *str, char *format, va_list arg)
{
    unsigned int i;
    unsigned int ZERO = 0;
    char *s;

    int position = 0;

    while (*format)
    {

        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
                i = va_arg(arg, int);
                str[position] = i;
		position++;
                break;

            case 'd':
                i = va_arg(arg, int);
                if (i < ZERO)
                {
                    i = -i;
                    str[position] = '-';
                }
                strcat(str, string_convert(i, 10));
                position += strlen(string_convert(i, 10));

                break;

            case 'o':
                i = va_arg(arg, unsigned int);
                strcat(str, string_convert(i, 8));
                position += strlen(string_convert(i, 8));
                break;

            case 's':
                s = va_arg(arg, char *);
                strcat(str, s);
                position += strlen(s);
                break;

            case 'x':
                i = va_arg(arg, unsigned int);
                strcat(str, string_convert(i, 16));
                position += strlen(string_convert(i, 16));
                break;
		
            default:
                str[position] = '%';
                position++;
                break;
            }
        }

        else
        {
            str[position] = *format;
	    position++;
        }

        format++;
    }
}

int atoi(char *str)
{
    int res = 0;

    int i;
    for (i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    return res;
}

size_t strlen(char *str)
{
    size_t i;

    for (i = 0; str[i] != '\0'; i++)
        ;
    return i;
}

char *strncat(char *dest, char *src, size_t n)
{
    size_t dest_length = strlen(dest);
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[dest_length + i] = src[i];
    }

    dest[dest_length + i] = '\0';

    return dest;
}

char *strcat(char *dest, char *src)
{
    return strncat(dest, src, strlen(src));
}
