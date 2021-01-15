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
#ifndef IDT_H
#define IDT_H
#include <stdint.h>
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#pragma once
extern void isr(void);
extern void isr_irq_master(void);
extern void isr_irq_slave(void);
struct idt_descriptor
{
    uint16_t offset_lo;
    uint16_t cs;
    uint8_t ist;
    uint8_t attrib;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t ignored;
} __attribute__((packed));
struct idt_pointer
{
    uint16_t size;
    uint64_t addr;
} __attribute__((packed));

void EmeraldSys_IDT_irq_remap(void);
void EmeraldSys_IDT_idt_register(uint16_t idx, void *handler, uint8_t cs, uint8_t attrib);
void EmeraldSys_IDT_isr_init(void);
void EmeraldSys_IDT_idt_load(void);
void EmeraldASM_sti();
void EmeraldSys_IDT_idt_init(void);
void EmeraldSys_IDT_irq_mask(unsigned char line);
void EmeraldSys_IDT_irq_clear_mask(unsigned char line);
#endif
