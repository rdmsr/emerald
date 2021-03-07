/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2021 Abb1x
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
 /*/

#include <libk/alloc.h>
#include <memory/pmm.h>
#include <memory/vmm.h>

Pagemap *kernel_map = NULL;

Pagemap *VMM_new_pagemap(void)
{
    Pagemap *pagemap = malloc(sizeof(pagemap));
    pagemap->top_level = PMM_allocate_zero(1);

    return pagemap;
}

static uintptr_t *get_next_level(uintptr_t *current_level, size_t entry)
{
    uintptr_t ret;

    if (current_level[entry] & 0x1)
    {
        ret = current_level[entry] & ~((uintptr_t)0xfff);
    }
    else
    {

        /* Allocate a table for the next paging level */
        ret = (uintptr_t)PMM_allocate_zero(1);
        if (ret == 0)
        {
            return NULL;
        }
        current_level[entry] = ret | 0b111;
    }

    return (void *)ret + MEM_OFFSET;
}

void VMM_map_page(Pagemap *pagemap, uintptr_t physical_address, uint64_t virtual_address, uintptr_t flags)
{
    uintptr_t level4 = (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
    uintptr_t level3 = (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
    uintptr_t level2 = (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
    uintptr_t level1 = (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

    uintptr_t *pml4, *pml3, *pml2, *pml1;

    pml4 = (void *)pagemap->top_level + MEM_OFFSET;

    pml3 = get_next_level(pml4, level4);

    pml2 = get_next_level(pml3, level3);

    pml1 = get_next_level(pml2, level2);

    pml1[level1] = physical_address | flags;
}

void VMM_switch_pagemap(Pagemap *pagemap)
{
    __asm__ volatile(
        "mov %0,%%cr3"
        :
        : "r"(pagemap->top_level)
        : "memory");
}

void VMM_init(void)
{

    module("VMM");
    kernel_map = VMM_new_pagemap();

    uintptr_t p;
    for (p = 0; p < 0x100000000; p += PAGE_SIZE)
    {
        VMM_map_page(kernel_map, p, p + MEM_OFFSET, 0x03);
    }

    log(INFO, "Mapped kernel!");

    VMM_switch_pagemap(kernel_map);
}
