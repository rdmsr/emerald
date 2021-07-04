/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <devices/com.h>
#include <emerald/str/str.h>

static int is_transmit_empty(SerialPort port)
{
    return asm_inb(port + 5) & 0x20;
}

static int serial_received(SerialPort port)
{
    return asm_inb(port + 5) & 1;
}

void com_initialize(SerialPort port)
{
    asm_outb(port + 1, 0x00);
    asm_outb(port + 3, 0x80);
    asm_outb(port + 0, 0x03);
    asm_outb(port + 1, 0x00);
    asm_outb(port + 3, 0x03);
    asm_outb(port + 2, 0xC7);
    asm_outb(port + 4, 0x0B);
}

void com_write_string(SerialPort port, char *str)
{
    while (*str)
    {
        com_putc(port, *str++);
    }
}

void com_putc(SerialPort port, char c)
{
    while (is_transmit_empty(port) == 0)
        ;
    asm_outb(port, c);
}

char com_getc(SerialPort port)
{
    while (serial_received(port) == 0)
        ;
    return asm_inb(port);
}
