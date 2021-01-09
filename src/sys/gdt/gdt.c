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
#include "gdt.h"
#include <debug-utilities/logger.h>
#include <libstr/string.h>
#include <stdint.h>
struct gdt_descriptor gdt[8];
struct gdt_pointer gdtr = {.limit = sizeof(gdt) - 1, .base = (uint64_t)gdt};

void EmeraldSys_GDT_gdt_load()
{
    asm volatile("lgdt %0"
                 :
                 : "m"(gdtr)
                 : "memory");
    asm volatile(R"(
    mov %%rsp, %%rax
    push $0x10
    push %%rax
    pushf
    push $0x8
    push $1f
    iretq
    1:
    mov $0x10, %%ax
    mov %%ax, %%ds
    mov %%ax, %%es
    mov %%ax, %%ss
    mov %%ax, %%fs
    mov %%ax, %%gs
                )"
                 :
                 :
                 : "rax", "memory");
}
void EmeraldSys_GDT_gdt_init()
{
    gdt[1] = (struct gdt_descriptor){.access = 0b10011010, .granularity = 0b00100000};
    gdt[2] = (struct gdt_descriptor){.access = 0b10010010, .granularity = 0};
    log(INFO, "Initializing GDT... \033[0;37mGDT[1] = {.access = 0b%s .granularity = 0b%s} GDT[2] = {.access = 0b%s .granularity = 0b%s%s %s", itoa(gdt[1].access, 2), itoa(gdt[1].granularity, 2), itoa(gdt[2].access, 2), itoa(gdt[2].granularity, 2), "}", "\033[1;0mDone");
    log(INFO, "Loading GDT...");
    EmeraldSys_GDT_gdt_load();
    log(INFO, "GDT loaded");
}
