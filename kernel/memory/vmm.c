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

Pagemap *VMM_new_pagemap()
{
    Pagemap *pagemap = (Pagemap *)malloc(sizeof(Pagemap));

    pagemap->pml4 = PMM_allocate_page();
    return pagemap;
}

uintptr_t lower_half(uintptr_t arg)
{
    /* offset is defined in link.ld */
    return arg - 0xffffffff80000000;
}
uintptr_t higher_half(uintptr_t arg)
{
    /* offset is defined in link.ld */
    return arg + 0xffffffff80000000;
}

uint64_t *walk_to_page_and_map(uint64_t *current, uint16_t index)
{
    if (current[index] == 0)
    {
        current[index] = (uint64_t)PMM_callocate_page();
        current[index] |= 0b11;
    }

    return (uint64_t *)current[index];
}

void VMM_map_page(Pagemap *page_map, uintptr_t physical_address, uint64_t virtual_address, uintptr_t flags)
{

    /* Paging levels */
    uintptr_t level4 = ((uintptr_t)virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
    uintptr_t level3 = ((uintptr_t)virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
    uintptr_t level2 = ((uintptr_t)virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
    uintptr_t level1 = ((uintptr_t)virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

    uint64_t *pml4 = page_map->pml4;

    uint64_t *pml3 = walk_to_page_and_map(pml4, level4);
    if (!pml3)
    {
        log(ERROR, "Pml3 is null");
        return;
    }

    uint64_t *pml2 = walk_to_page_and_map(pml3, level3);
    if (!pml2)
    {
        log(ERROR, "pml2 is null");
        return;
    }

    uint64_t *pml1 = walk_to_page_and_map(pml2, level2);

    if (!pml1)
    {
        log(ERROR, "pml1 is null");
        return;
    }
    pml1[level1] = physical_address | flags;
}
void VMM_switch_pagemap(Pagemap *map)
{
    __asm__("mov %0, %%cr3" ::"r"(map->pml4)
            : "memory");
}
void VMM_init()
{
    module("VMM");

    Pagemap *kernel_map = VMM_new_pagemap();

    log(INFO, "Kernel pagemap created!");

    uint64_t i;
    for (i = 0; i < 0x100000000; i += PAGE_SIZE)
    {
        VMM_map_page(kernel_map, i, i + 0xffffffff80000000, 0b11);
    }

    log(INFO, "Mapped kernel");

    VMM_switch_pagemap(kernel_map);

    log(INFO, "VMM initialized!");
}
