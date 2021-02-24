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

#include "rtc.h"
#include <libk/io.h>

/* Check if RTC is updating */
static int is_updating()
{
    IO_outb(0x70, 0x0A);
    return IO_inb(0x71) & 0x80;
}

static unsigned char read(int reg)
{
    while (is_updating())
        ;
    IO_outb(0x70, reg);

    return IO_inb(0x71);
}

unsigned char RTC_get_seconds()
{
    unsigned char seconds = read(0);
    unsigned char second = (seconds & 0x0F) + ((seconds / 16) * 10);
    return second;
}

unsigned char RTC_get_minutes()
{
    unsigned char minutes = read(0x2);
    unsigned char minute = (minutes & 0x0F) + ((minutes / 16) * 10);
    return minute;
}

unsigned char RTC_get_hours()
{
    unsigned char hours = read(0x4);
    unsigned char hour = ((hours & 0x0F) + (((hours & 0x70) / 16) * 10)) | (hours & 0x80);
    return hour;
}

Time RTC_get_time()
{
    Time time;
    time.hour = RTC_get_hours();
    time.minute = RTC_get_minutes();
    time.second = RTC_get_seconds();
    return time;
}

DateTime RTC_get_date_time()
{
    DateTime date_time;

    date_time.day = read(0x7);
    date_time.month = read(0x8);
    date_time.year = read(0x9);

    date_time.time = RTC_get_time();

    return date_time;
}
