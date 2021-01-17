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
#include "vmm.h"
#include "../physical/pmm.h"
#include <debug-utilities/logger.h>
/*

Virtual Memory Manager
----------------------

How does it work?

in my VMM, I use a memory management scheme called 'paging' here is how it works:

The virtual memory is split in equally-sized blocks called "pages" they are usually 4kb long or 4096 bytes.

These pages are then mapped on a pagemap (also called pagetable)

*/

/* In this function we create a pagemap with pages of size 4096 */
void EmeraldMem_VMM_create_pagemap(pagemap_t *map)
{
    /* We allocate the page */
    uint64_t page = EmeraldMem_PMM_allocz(1);
    uint64_t *pml4 = (uint64_t*)page;
    map->pml4 = pml4;
}
uint64_t *walk_to_page_and_map(uint64_t *current, uint16_t index)
{
    if (current[index] == 0)
    {
        current[index] = EmeraldMem_PMM_allocate_block(4096);
        current[index] |= 0b11;
    }

    return (uint64_t *)current[index];
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
void EmeraldMem_VMM_map_page(pagemap_t *page_map, uintptr_t physical_adress, uint64_t virtual_adress, uintptr_t flags)
{
    /* get physical adress */
    physical_adress = lower_half(virtual_adress);

    /* Paging levels */
    uint16_t level1 = virtual_adress >> 12;
    uint16_t level2 = virtual_adress >> 21;
    uint16_t level3 = virtual_adress >> 30;
    uint16_t level4 = virtual_adress >> 39;
    uint64_t *root = page_map->pml4;

    walk_to_page_and_map(root, level4);

    walk_to_page_and_map(root, level3);

    uint64_t *pml1 = walk_to_page_and_map(root, level2);

    pml1[level1] = physical_adress | flags;
}

void EmeraldMem_VMM_initialize()
{
    /* This function maps the first 4gb of ram */
    pagemap_t *page_map = page_map;

    EmeraldMem_VMM_create_pagemap(page_map);

    //    uint64_t *root = page_map->pml4;
    // uintptr_t root_lower_half = lower_half(*root);
    /* Maps the first 4 gb */
    for (uint64_t i = 0; i < 0x100000000; i += 0x1000)
    {
        EmeraldMem_VMM_map_page(page_map, i, higher_half(i), 0b11);
    }
    uint64_t pml4_lower_half = lower_half(*page_map->pml4);

    asm volatile (
        "mov %0,%%cr3"
        :
        : "r" (pml4_lower_half)
        : "memory"
	);
    log(INFO, "Mapped Kernel");
}
void EmeraldMem_VMM_unmap_page(pagemap_t *page_map, uint64_t virtual_adress)
{
    uint16_t level1 = virtual_adress >> 12;
    uint16_t level2 = virtual_adress >> 21;
    uint16_t level3 = virtual_adress >> 30;
    uint16_t level4 = virtual_adress >> 39;

    uint64_t *root = page_map->pml4;
    walk_to_page_and_map(root, level4);
    walk_to_page_and_map(root, level3);
    uint64_t *pml1 = walk_to_page_and_map(root, level2);

    pml1[level1] = 0;
}

void EmeraldMem_set_bit(uint8_t *num, uint8_t bit, uint8_t state)
{
    if (state == 0)
    {
        *num &= ~(1 << bit);
    }
    else if (state == 1)
    {
        *num = (*num | (1 << bit));
    }
}
