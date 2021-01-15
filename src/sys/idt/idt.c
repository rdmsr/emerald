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
void EmeraldSys_IDT_idt_register(uint16_t idx, void *handler, uint8_t cs, uint8_t attrib)
{
    uint64_t ptr = (uint64_t)handler;
    idt[idx] = (struct idt_descriptor){
        .offset_lo = ptr, .cs = cs, .attrib = attrib, .offset_mid = ptr >> 16, .offset_hi = ptr >> 32};
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

    for (int i = 0; i < 0x22; i++)
    {
        EmeraldSys_IDT_idt_register(i, isr, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
    }
    EmeraldSys_IDT_idt_register(0x20, isr_irq_master, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);

    EmeraldSys_IDT_idt_register(0x21, isr, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);

    for (int i = 0x23; i < 0x28; i++)
    {
        EmeraldSys_IDT_idt_register(i, isr_irq_master, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
    }
    for (int i = 0x28; i < 0x2F; i++)
    {
        EmeraldSys_IDT_idt_register(i, isr_irq_slave, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
    }
    /* Keyboard IRQ */
    EmeraldSys_IDT_idt_register(0x31, EmeraldDevices_keyboard_Keyboard_handler_main, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
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
