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
 */

#include "pmm.h"
#include <stdbool.h>

uint8_t *bitmap;
size_t last_used_index = 0;
uintptr_t highest_page = 0;

void *memset(void *bufptr, int value, size_t size)
{
    unsigned char *buf = (unsigned char *)bufptr;
    size_t i;
    for (i = 0; i < size; i++)
        buf[i] = (unsigned char)value;
    return bufptr;
}

void PMM_init(struct stivale2_mmap_entry *memory_map, size_t memory_entries)
{
    module("PMM");

    log(INFO, "Memory Adress: %x", (uint64_t)memory_map);

    size_t i, j, length, i_entry;

    for (i_entry = 0; i_entry < memory_entries; i_entry++)
    {
        struct stivale2_mmap_entry entry = memory_map[i_entry];

        log(INFO, "[Entry %d] [%x - %x]: size: %x, type: %d", i_entry, entry.base, entry.base + entry.length, entry.length, entry.type);

        if (entry.type != STIVALE2_MMAP_USABLE && entry.type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE && entry.type != STIVALE2_MMAP_KERNEL_AND_MODULES)
            continue;

        uintptr_t top = entry.base + entry.length;

        if (top > highest_page)
            highest_page = top;
    }

    size_t memory_size = highest_page + (PAGE_SIZE - 1) / PAGE_SIZE;
    size_t bitmap_size = memory_size / 8;

    for (i = 0; i < memory_entries; i++)
    {
        struct stivale2_mmap_entry entry = memory_map[i];

        if (entry.type != STIVALE2_MMAP_USABLE)
            continue;

        if (entry.length >= bitmap_size)
        {
            bitmap = (uint8_t *)entry.base + MEM_OFFSET;
            memset(bitmap, 0xff, bitmap_size);

            entry.base += bitmap_size;
            entry.length -= bitmap_size;

            break;
        }
    }

    for (j = 0; j < memory_entries; j++)
    {
        struct stivale2_mmap_entry entry = memory_map[j];
        if (entry.type != STIVALE2_MMAP_USABLE)
            continue;

        for (length = 0; length < entry.length; length += PAGE_SIZE)
            CLEARBIT((entry.base + length) / PAGE_SIZE);
    }

    log(INFO, "PMM initialized!");
}

void *PMM_inner_allocate(size_t count, size_t limit)
{
    size_t p = 0;
    size_t i;
    while (last_used_index < limit)
    {
        if (bitmap[last_used_index++] == false)
        {
            if (p++ == count)
            {
                size_t page = last_used_index - count;

                for (i = page; i < last_used_index; i++)
                    SETBIT(i);

                return (void *)(page * PAGE_SIZE);
            }
        }
        else
        {
            p = 0;
        }
    }
    return NULL;
}

void *PMM_allocate_page()
{
    return PMM_allocate_pages(1);
}

void *PMM_allocate_pages(size_t count)
{
    size_t length = last_used_index;

    void *address = PMM_inner_allocate(count, highest_page / PAGE_SIZE);

    if (address == NULL)
    {
        last_used_index = 0;
        address = PMM_inner_allocate(count, length);
    }

    return address;
}

void *PMM_callocate_page()
{
    return PMM_callocate_pages(1);
}

void *PMM_callocate_pages(size_t count)
{
    char *address = (char *)PMM_allocate_pages(count);

    if (address == NULL)
        return NULL;

    uint64_t *ptr = (uint64_t *)(address + MEM_OFFSET);

    size_t i;

    for (i = 0; i < count * (PAGE_SIZE / sizeof(uint64_t)); i++)
        ptr[i] = 0;

    return address;
}

void PMM_free_page(void *address)
{
    PMM_free_pages(address, 1);
}

void PMM_free_pages(void *address, size_t count)
{
    uint64_t startPage = (uint64_t)address / PAGE_SIZE;
    size_t i;
    for (i = startPage; i < startPage + count; i++)
        CLEARBIT(i);
}
