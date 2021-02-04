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
#ifndef ASCII_H
#define ASCII_H
#include <devices/rtc/rtc.h>
#include <devices/video/vbe.h>
#include <devices/serial/serial.h>
char ascii_art[] = "\e[0;32m  _____                         _     _ \n"
                   " |  ___|                       | |   | |\n"
                   " | |__ _ __ ___   ___ _ __ __ _| | __| |\n"
                   " |  __| '_ ` _ \\ / _ \\ '__/ _` | |/ _` |\n"
                   " | |__| | | | | |  __/ | | (_| | | (_| |\n"
                   " |____\\_| |_| |_|\\___|_|  \\__,_|_|\\__,_|\n\e[0m"
                   " ───────────────────────────────────────\n"
                   "  Made 2020-2021 EmeraldOS contributors\n"
                   "\n";

char *strcpy(char *destination, const char *source)
{
    if (!destination)
        return NULL;

    char *ptr = destination;

    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = '\0';

    return ptr;
}



void set_ascii()
{
  Serial_write_string(ascii_art);
  if (RTC_get_hours() < 12)    
    {
        Serial_write_string("Good Morning!\n");
    }
    if (RTC_get_hours() >= 12 && RTC_get_hours() < 18)
    {
        Serial_write_string("Good Afternoon!\n");
    }
    if (RTC_get_hours() >= 18)
    {
        Serial_write_string("Good Evening!\n");
    }
}
#endif
