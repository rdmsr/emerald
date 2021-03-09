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

#include <libk/io.h>

unsigned char IO_inb(unsigned short port)
{
    unsigned char ret;
    __asm__ volatile("inb %1, %0"
                     : "=a"(ret)
                     : "Nd"(port));
    return ret;
}

void IO_outb(uint16_t port, uint8_t value)
{
    __asm__ volatile("outb %0, %1"
                     :
                     : "a"(value), "Nd"(port));
}

void IO_outl(uint16_t port, uint32_t value)
{
    __asm__ volatile("out %%eax,%%dx" ::"a"(value), "d"(port));
}

uint32_t IO_inl(uint16_t port)
{
    uint32_t ret;
    __asm__ volatile("in %%dx,%%eax"
                     : "=a"(ret)
                     : "d"(port));
    return ret;
}

void IO_outw(unsigned short port, unsigned short value)
{
    __asm__ volatile("outw %%ax,%%dx"
                     :
                     : "dN"(port), "a"(value));
}
