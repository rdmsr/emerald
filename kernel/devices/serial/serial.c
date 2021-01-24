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
#include <devices/serial/serial.h>

static int is_transmit_empty()
{
    return IO_inb(PORT + 5) & 0x20;
}

static int serial_received()
{
    return IO_inb(PORT + 5) & 1;
}

void Serial_init()
{
    IO_outb(PORT + 1, 0x00);
    IO_outb(PORT + 3, 0x80);
    IO_outb(PORT + 0, 0x03);
    IO_outb(PORT + 1, 0x00);
    IO_outb(PORT + 3, 0x03);
    IO_outb(PORT + 2, 0xC7);
    IO_outb(PORT + 4, 0x0B);
}

void Serial_write(char c)
{
    while (is_transmit_empty() == 0)
        ;
    return IO_outb(PORT, c);
}

char Serial_read()
{
    while (serial_received() == 0)
        ;
    return IO_inb(PORT);
}

void Serial_write_string(char *string)
{
    while (*string)
    {
        Serial_write(*string++);
    }
}
