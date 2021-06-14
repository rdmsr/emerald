/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_COM_H
#define KERNEL_COM_H

#include <stdint.h>
#include <arch/asm.h>

typedef enum
{
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8
} SerialPort;

void com_initialize(SerialPort port);
void com_write_string(SerialPort port, char* str);
void com_putc(SerialPort port, char c);
char com_getc(SerialPort port);

#endif
