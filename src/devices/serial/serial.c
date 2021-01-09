/*
MIT License

Copyright (c) 2021 Abb1x

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "serial.h"
#include <libasm/asm.h>
#include <libstr/string.h>
#define PORT 0x3f8

void EmeraldDevices_Serial_init_serial()
{
    EmeraldASM_outb(PORT + 1, 0x00);
    EmeraldASM_outb(PORT + 3, 0x80);
    EmeraldASM_outb(PORT + 0, 0x03);
    EmeraldASM_outb(PORT + 1, 0x00);
    EmeraldASM_outb(PORT + 3, 0x03);
    EmeraldASM_outb(PORT + 2, 0xC7);
    EmeraldASM_outb(PORT + 4, 0x0B);
}
int EmeraldDevices_Serial_serial_received()
{
    return EmeraldASM_inb(PORT + 5) & 1;
}
char EmeraldDevices_Serial_read_serial()
{
    while (EmeraldDevices_Serial_serial_received() == 0)
        ;
    return EmeraldASM_inb(PORT);
}

int EmeraldDevices_Serial_is_transmit_empty()
{
    return EmeraldASM_inb(PORT + 5) & 0x20;
}
void EmeraldDevices_Serial_write_serial(char a)
{
    while (EmeraldDevices_Serial_is_transmit_empty() == 0)
        ;
    EmeraldASM_outb(PORT, a);
}

void puts(char *string)
{
    for (unsigned int i = 0; i < strlen(string); i++)
    {
        char output = string[i];
        EmeraldDevices_Serial_write_serial((int)output);
    }
}
void putchar(char character)
{
    EmeraldDevices_Serial_write_serial((int)(character));
}
