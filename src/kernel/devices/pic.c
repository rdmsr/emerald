/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "pic.h"
#include <arch/asm.h>
#include <emerald/log.h>

void pic_wait(void)
{
    asm volatile("jmp 1f\n\t"
                 "1:\n\t"
                 "    jmp 2f\n\t"
                 "2:");
}

void pic_disable(void)
{
    asm_outb(PIC2_DATA, 0xff);
    asm_outb(PIC1_DATA, 0xff);
}

void pic_remap(void)
{
    asm_outb(0x20, 0x11);
    asm_outb(0xA0, 0x11);

    asm_outb(0x21, 0x20);
    asm_outb(0xA1, 0x28);

    asm_outb(0x21, 0x04);
    asm_outb(0xA1, 0x02);

    asm_outb(0x21, 0x01);
    asm_outb(0xA1, 0x01);

    asm_outb(0x21, 0x0);
    asm_outb(0xA1, 0x0);
    log("PIC remapped");
}

void pic_initialize(void)
{
  pic_remap();
}

void pic_eoi(int intno)
{
    if (intno >= 40)
    {
        asm_outb(PIC2, 0x20);
    }

    asm_outb(PIC1, 0x20);
}


