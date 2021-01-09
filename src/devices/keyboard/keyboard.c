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
#include "keyboard.h"
#include <debug-utilities/logger.h>
#include <devices/video/vbe/vbe.h>
#include <devices/video/colors.h>
#include <devices/RTC/rtc.h>
#include <libasm/asm.h>
#include <libint/int.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define ENTER_KEY_CODE 0x1C
unsigned char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
    '9', '0', '-', '=', '\0',                         /* Backspace */
    '\t',                                             /* Tab */
    'q', 'w', 'e', 'r',                               /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
    0,                                                /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`', 0,                                     /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
    'm', ',', '.', '/', 0,                            /* Right shift */
    '*',
    0,   /* Alt */
    ' ', /* Space bar */
    0,   /* Caps lock */
    0,   /* 59 - F1 key ... > */
    0, 0, 0, 0, 0, 0, 0, 0,
    0, /* < ... F10 */
    0, /* 69 - Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up Arrow */
    0, /* Page Up */
    '-',
    0, /* Left Arrow */
    0,
    0, /* Right Arrow */
    '+',
    0, /* 79 - End key*/
    0, /* Down Arrow */
    0, /* Page Down */
    0, /* Insert Key */
    0, /* Delete Key */
    0, 0, 0,
    0, /* F11 Key */
    0, /* F12 Key */
    0, /* All other keys are undefined */
};
unsigned int current_location = 0;

void EmeraldDevices_keyboard_Keyboard_init(void)
{
    EmeraldASM_outb(0x21, 0xFD);
}
void PIC_sendEOI(unsigned char irq)
{
    if (irq >= 8)
        EmeraldASM_outb(PIC2_COMMAND, PIC_EOI);

    EmeraldASM_outb(PIC1_COMMAND, PIC_EOI);
}
void EmeraldDevices_keyboard_Keyboard_handler_main()
{
    char keycode;
    EmeraldASM_outb(0x20, 0x20);
    EmeraldASM_inb(KEYBOARD_STATUS_PORT);
    keycode = EmeraldASM_inb(KEYBOARD_DATA_PORT);
    color_t white = {255,255,255};

    if (keycode < 0)
    {
        PIC_sendEOI(0);
        return;
    }

    if (keycode == ENTER_KEY_CODE)
    {
        EmeraldDevices_VBE_put('\n',white);
        return;
    }
    if (keycode == DELETE_KEY_CODE)
    {
        EmeraldDevices_VBE_put('\0',white);
        return;
    }
    EmeraldDevices_VBE_put(keyboard_map[(unsigned char)keycode],white);
    EmeraldDevices_VGA_update_cursor(current_location / 2, 0);
}
