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

/* stivale2_mmap_entry */
#include "pmm.h"
#include <stdbool.h>
#include <stddef.h>

/* Here, we define the bitmap as NULL since it's a pointer */
uint8_t *bitmap = NULL;
uint64_t last_free_page = 0;
size_t memory_size;

static uint64_t get_bitmap_array_index(uint64_t page_addr)
{
    return page_addr / 8; /* The bitmap is a uint8_t so we use 8 to divide */
}

static uint64_t get_bitmap_bit_index(uint64_t page_addr)
{
    return page_addr % 8;
}

/* These functions are for setting/clearing bits into the bitmap */
static void bitmap_set_bit(uint64_t page_addr)
{
    uint64_t bit = get_bitmap_bit_index(page_addr);
    uint64_t byte = get_bitmap_array_index(page_addr);

    bitmap[byte] |= (1 << bit);
}

static void bitmap_clear_bit(uint64_t page_addr)
{
    uint64_t bit = get_bitmap_bit_index(page_addr);
    uint64_t byte = get_bitmap_array_index(page_addr);

    bitmap[byte] &= ~(1 << bit);
}

static bool bitmap_is_bit_set(uint64_t page_addr)
{

    uint64_t bit = get_bitmap_bit_index(page_addr);
    uint64_t byte = get_bitmap_array_index(page_addr);
    return bitmap[byte] & (1 << bit);
}

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

    /* Here, we want to get the size of the memory and the size of the bitmap */
    size_t i = 0, i_entry;
    uintptr_t highest_page = 0;

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

    memory_size = highest_page + (PAGE_SIZE - 1) / PAGE_SIZE;
    size_t bitmap_size = memory_size / 8;

    /* Here, we are finding a place to put the bitmap in */

    size_t m;

    for (m = 0; m < memory_entries && bitmap == NULL; m++)
    {
        log(INFO, "Finding a place for the bitmap...");
        struct stivale2_mmap_entry entry = memory_map[i];

        if (entry.type == STIVALE2_MMAP_USABLE && entry.length > bitmap_size)
        {
            bitmap = (uint8_t *)entry.base + MEM_OFFSET;
            log(INFO, "Found place for the bitmap at %x", bitmap);
        }
    }

    uint64_t free_memory = 0;

    memset(bitmap, 0xff, bitmap_size); /* Setting all pages as used */
    size_t n;
    for (n = 0; n < memory_entries; n++)
    {
        struct stivale2_mmap_entry entry = memory_map[i];
        if (entry.type == STIVALE2_MMAP_USABLE)
        {
            uint64_t j;
            for (j = 0; j < entry.length; j += PAGE_SIZE)
            {
                bitmap_clear_bit((entry.base + j) / PAGE_SIZE);
                free_memory += PAGE_SIZE;
            }
        }
    }

    uint64_t bitmap_start = (uint64_t)bitmap;

    uint64_t bitmap_end = bitmap_start + bitmap_size;

    uint64_t a;
    for (a = bitmap_start; a <= bitmap_end; a += PAGE_SIZE)
    {
        bitmap_set_bit(a / PAGE_SIZE);
    }
}

uint64_t find_free_pages(uint64_t count)
{
    uint64_t free_count = 0;

    size_t i;
    for (i = last_free_page; i < (memory_size / PAGE_SIZE); i++)
    {
        if (!bitmap_is_bit_set(i))
        {
            free_count++;
            if (free_count == count)
            {
                last_free_page = i; /* Last free page */
                return i;
            }
        }
        else
        {
            free_count = 0;
        }
    }

    if (last_free_page != 0)
    {
        last_free_page = 0;
        return find_free_pages(count);
    }

    return -1; /* There is no free page */
}

void *PMM_allocate_pages(uint64_t count)
{
    uint64_t page = find_free_pages(count);

    uintptr_t i;
    for (i = page; i < count + page; i++)
    {
        bitmap_set_bit(i);
    }
    return (void *)(page * PAGE_SIZE);
}

void PMM_free_pages(void *addr, uint64_t page_count)
{
    uint64_t target = ((uint64_t)addr) / PAGE_SIZE;

    uintptr_t i;
    for (i = target; i <= target + page_count; i++)
    {
        bitmap_clear_bit(i);
    }
}

void *PMM_allocate_zero(uint64_t count)
{
    void *d = PMM_allocate_pages(count);
    uint64_t *pages = (uint64_t *)(d + MEM_OFFSET);

    uint64_t i;

    for (i = 0; i < (count * PAGE_SIZE); i++)
    {
        pages[i] = 0;
    }
    return d;
}
