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
#ifndef LOGGER_H
#define LOGGER_H
#include <devices/RTC/rtc.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
enum status
{
    INFO,
    WARNING,
    DEBUG,
    PANIC,
    ERROR
};
void log(int status, char *format, ...);
extern unsigned char second;
extern unsigned char minute;
extern unsigned char hour;
extern unsigned char day;
extern unsigned char month;
extern unsigned int year;
extern unsigned char century;
extern unsigned char last_second;
extern unsigned char last_minute;
extern unsigned char last_hour;
extern unsigned char last_day;
extern unsigned char last_month;
extern unsigned char last_year;
extern unsigned char last_century;
extern unsigned char registerB;
#endif
