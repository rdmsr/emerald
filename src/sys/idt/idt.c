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
#include <sys/exceptions.h>
#include <stdint.h>
static struct idt_descriptor idt[256];
extern void kb_handler();
extern void pit_handler();
static struct idt_pointer idtr = {.size = 256 * sizeof(struct idt_descriptor), .addr = (uint64_t)idt};
void EmeraldSys_IDT_irq_remap(void)
{
    EmeraldASM_outb(0x20, 0x11);
    EmeraldASM_outb(0xA0, 0x11);

    EmeraldASM_outb(0x21, 0x20);
    EmeraldASM_outb(0xA1, 0x28);

    EmeraldASM_outb(0x21, 0x04);
    EmeraldASM_outb(0xA1, 0x02);

    EmeraldASM_outb(0x21, 0x01);
    EmeraldASM_outb(0xA1, 0x01);

    EmeraldASM_outb(0x21, 0x0);
    EmeraldASM_outb(0xA1, 0x0);
    log(INFO, "IRQs Remapped");
}
static struct idt_descriptor idt_make_entry(uint64_t offset)
{
    return (struct idt_descriptor){
        .selector = 0x08,
        .offset_lo = offset & 0xFFFF,
        .offset_mid = (offset >> 16) & 0xFFFF,
        .offset_hi = (offset >> 32) & 0xFFFFFFFF,
        .ist = 0,
        .zero = 0,
        .type_attr = 0x8e};
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
    idt[0] = idt_make_entry((uint64_t)&exc_0);
    idt[1] = idt_make_entry((uint64_t)&exc_1);
    idt[2] = idt_make_entry((uint64_t)&exc_2);
    idt[3] = idt_make_entry((uint64_t)&exc_3);
    idt[4] = idt_make_entry((uint64_t)&exc_4);
    idt[5] = idt_make_entry((uint64_t)&exc_5);
    idt[6] = idt_make_entry((uint64_t)&exc_6);
    idt[7] = idt_make_entry((uint64_t)&exc_7);
    idt[8] = idt_make_entry((uint64_t)&exc_8);
    idt[9] = idt_make_entry((uint64_t)&exc_9);
    idt[10] = idt_make_entry((uint64_t)&exc_10);
    idt[11] = idt_make_entry((uint64_t)&exc_11);
    idt[12] = idt_make_entry((uint64_t)&exc_12);
    idt[13] = idt_make_entry((uint64_t)&exc_13);
    idt[14] = idt_make_entry((uint64_t)&exc_14);
    idt[15] = idt_make_entry((uint64_t)&exc_15);
    idt[16] = idt_make_entry((uint64_t)&exc_16);
    idt[17] = idt_make_entry((uint64_t)&exc_17);
    idt[18] = idt_make_entry((uint64_t)&exc_18);
    idt[19] = idt_make_entry((uint64_t)&exc_19);
    idt[20] = idt_make_entry((uint64_t)&exc_20);
    idt[30] = idt_make_entry((uint64_t)&exc_30);
    
    idt[32] = idt_make_entry((uint64_t)&pit_handler);
    idt[33] = idt_make_entry((uint64_t)&kb_handler);

    for (int i = 0x23; i < 0x28; i++)
    {
        idt[i] = idt_make_entry((uint64_t)&isr_irq_master);
    }
    for (int i = 0x28; i < 0x2F; i++)
    {
        idt[i] = idt_make_entry((uint64_t)&isr_irq_slave);
    }
}

void EmeraldSys_IDT_idt_load(void)
{
    asm volatile("lidt %0"
                 :
                 : "m"(idtr));
}
void EmeraldSys_IDT_idt_init(void)
{
    EmeraldSys_IDT_isr_init();
    EmeraldSys_IDT_idt_load();
    EmeraldASM_sti();
    log(INFO, "Initializing IDT...\033[0;37m loading ISR... loading IDT... \033[1;0mDone");
}
