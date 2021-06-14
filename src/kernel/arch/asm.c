/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <arch/asm.h>

void cli(void)
{
    __asm__ volatile("cli");
}

void sti(void)
{
    __asm__ volatile("sti");
}

void hlt(void)
{
    __asm__ volatile("hlt");
}

uint8_t inb(uint16_t port)
{
    uint8_t data;
    __asm__ volatile("inb %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

void outb(uint16_t port, uint8_t data)
{
    __asm__ volatile("outb %0, %1"
                     :
                     : "a"(data), "Nd"(port));
}
