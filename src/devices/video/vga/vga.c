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
#include "vga.h"
#include <debug-utilities/logger.h>
#include <devices/serial/serial.h>
#include <libasm/asm.h>
#include <mem/virtual/vmm.h>
unsigned int current_loc = 0;
char *vidptr = (char *)0xb8000;
void kprint(const char *str, int color)
{
    unsigned int i = 0;
    while (str[i] != '\0' && str[i] != '\n')

    {

        vidptr[current_loc++] = str[i++];
        vidptr[current_loc++] = color;
    }

    if (str[i] == '\n')
    {
        kprint_newline();
    }
}
void kprint_newline(void)
{
    unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
    current_loc = current_loc + (line_size - current_loc % (line_size));
}
void kprint_load(char *str, bool error)
{
    if (error == false)
    {

        kprint("[ ", 7);
        kprint("OK ", 10);
        kprint("] ", 7);
        kprint(str, 0x07);
        kprint(" loaded", 0x07);
        kprint_newline();
    }
    else if (error == true)
    {
        kprint("[ ", 7);
        kprint("ERROR ", 12);
        kprint("] ", 7);
        kprint(str, 0x07);
        kprint(" failed to load", 0x07);
        kprint_newline();
    }
}

void clear_screen(void)
{
    unsigned int i = 0;
    while (i < SCREENSIZE)
    {
        vidptr[i++] = ' ';
        vidptr[i++] = 0xAA;
    }
}
void EmeraldDevices_VGA_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    EmeraldASM_outb(0x3D4, 0x0A);
    EmeraldASM_outb(0x3D5, (EmeraldASM_inb(0x3D5) & 0xC0) | cursor_start);

    EmeraldASM_outb(0x3D4, 0x0B);
    EmeraldASM_outb(0x3D5, (EmeraldASM_inb(0x3D5) & 0xE0) | cursor_end);
}
void EmeraldDevices_VGA_update_cursor(int x, int y)
{
    uint16_t pos = y * 80 + x;

    EmeraldASM_outb(0x3D4, 0x0F);
    EmeraldASM_outb(0x3D5, (uint8_t)(pos & 0xFF));
    EmeraldASM_outb(0x3D4, 0x0E);
    EmeraldASM_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
