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
#include "idt.h"
#include <debug-utilities/logger.h>
#include <devices/keyboard/keyboard.h>
#include <libasm/asm.h>
#include <proc/PIT.h>
#include <stdint.h>
static struct idt_descriptor idt[256];
static struct idt_pointer idtr = {.size = sizeof(idt) - 1, .addr = (uint64_t)idt};
#define io_wait() ({ EmeraldASM_outb(0x80, 0x00); })
void EmeraldSys_IDT_irq_remap(void)
{

    EmeraldASM_outb(0x20, 0x11);
    EmeraldASM_outb(0xa0, 0x11);
    EmeraldASM_outb(0x21, 0x20);
    EmeraldASM_outb(0xa1, 0x28);
    EmeraldASM_outb(0x21, 4);
    EmeraldASM_outb(0xa1, 2);

    EmeraldASM_outb(0x21, 0x01);
    EmeraldASM_outb(0xa1, 0x01);
    EmeraldASM_outb(0x21, 0x00);
    EmeraldASM_outb(0xa1, 0x00);
    
   log(INFO,"Remapped IRQs");
}


struct idt_descriptor EmeraldSys_IDT_make_entry(uint64_t offset)
{
    return (struct idt_descriptor){
        .cs = KERNEL_CODE_SEGMENT_OFFSET,
        .attrib = INTERRUPT_GATE,
        .offset_lo = offset & 0xFFFF,
        .offset_mid = (offset >> 16) & 0xFFFF,
        .offset_hi = (offset >> 32) & 0xFFFFFFFF};
}
void idt_set_handler(uint8_t vector, void *handler)
{
    idt[vector] = EmeraldSys_IDT_make_entry((uint64_t)handler);
}
void EmeraldSys_IDT_irq_mask(unsigned char line)
{
    uint16_t port;
    uint8_t value;

    if (line < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        line -= 8;
    }
    value = EmeraldASM_inb(port) | (1 << line);
    EmeraldASM_outb(port, value);
}
void EmeraldSys_IDT_irq_clear_mask(unsigned char line)
{
    uint16_t port;
    uint8_t value;

    if (line < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        line -= 8;
    }
    value = EmeraldASM_inb(port) & ~(1 << line);
    EmeraldASM_outb(port, value);
}

void EmeraldSys_IDT_isr_init(void)
{
    idt[0] = EmeraldSys_IDT_make_entry((uint64_t)&isr0);
    idt[1] = EmeraldSys_IDT_make_entry((uint64_t)&isr1);
    idt[2] = EmeraldSys_IDT_make_entry((uint64_t)&isr2);
    idt[3] = EmeraldSys_IDT_make_entry((uint64_t)&isr3);
    idt[4] = EmeraldSys_IDT_make_entry((uint64_t)&isr4);
    idt[5] = EmeraldSys_IDT_make_entry((uint64_t)&isr5);
    idt[6] = EmeraldSys_IDT_make_entry((uint64_t)&isr6);
    idt[7] = EmeraldSys_IDT_make_entry((uint64_t)&isr7);
    idt[8] = EmeraldSys_IDT_make_entry((uint64_t)&isr8);
    idt[10] = EmeraldSys_IDT_make_entry((uint64_t)&isr10);
    idt[11] = EmeraldSys_IDT_make_entry((uint64_t)&isr11);
    idt[12] = EmeraldSys_IDT_make_entry((uint64_t)&isr12);
    idt[13] = EmeraldSys_IDT_make_entry((uint64_t)&isr13);
    idt[14] = EmeraldSys_IDT_make_entry((uint64_t)&isr14);
    idt[16] = EmeraldSys_IDT_make_entry((uint64_t)&isr16);
    idt[17] = EmeraldSys_IDT_make_entry((uint64_t)&isr17);
    idt[18] = EmeraldSys_IDT_make_entry((uint64_t)&isr18);
    idt[19] = EmeraldSys_IDT_make_entry((uint64_t)&isr19);
    idt[20] = EmeraldSys_IDT_make_entry((uint64_t)&isr20);
    idt[30] = EmeraldSys_IDT_make_entry((uint64_t)&isr30);
}

void EmeraldSys_IDT_idt_load(void)
{
    asm volatile("lidt %0"
                 :
                 : "m"(idtr));
}
void EmeraldSys_IDT_idt_init(void)
{
    EmeraldSys_IDT_irq_remap();

    EmeraldSys_IDT_isr_init();
    EmeraldSys_IDT_idt_load();
    EmeraldASM_sti();
    log(INFO, "Initializing IDT...\033[0;37m loading ISR... loading IDT... \033[1;0mDone");
}
