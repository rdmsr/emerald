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
#include "logging.h"
#include <devices/rtc/rtc.h>
#include <devices/serial/serial.h>

char *current_module = "";
void log(int status, char *format, ...)
{
    char *string;
    switch (status)
    {
    case INFO:
        string = "%d:%d:%d [\033[1;34m INFO \033[1;34m\033[0m]\033[1;34m %s:\033[0m%s";
        break;
    case DEBUG:
        string = "%d:%d:%d [\033[1;36m DEBUG \033[1;36m\033[0m]\033[1;36m %s:\033[0m%s";
        break;
    case WARNING:
        string = "%d:%d:%d [\033[1;33m WARNING \033[1;33m\033[0m]\033[1;33m %s:\033[0m%s";
        break;
    case ERROR:
        string = "%d:%d:%d [\033[1;31m ERROR \033[1;31m\033[0m]\033[1;31m %s:\033[0m%s";
        break;
    case PANIC:
        string = "%d:%d:%d [\033[1;31m PANIC \033[1;31m\033[0m]\033[1;31m %s:\033[0m%s";
        break;
    default:
        string = " string";
    };

    printf(string, RTC_get_hours(), RTC_get_minutes(), RTC_get_seconds(), current_module, " ");

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

    Serial_write('\n');
}
