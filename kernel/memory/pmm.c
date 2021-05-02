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
#include <stddef.h>

/* Bit macros. thanks AtieP */
#define ALIGN_UP(__number) (((__number) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ALIGN_DOWN(__number) ((__number) & ~(PAGE_SIZE - 1))

#define BIT_SET(__bit) (bitmap[(__bit) / 8] |= (1 << ((__bit) % 8)))
#define BIT_CLEAR(__bit) (bitmap[(__bit) / 8] &= ~(1 << ((__bit) % 8)))
#define BIT_TEST(__bit) ((bitmap[(__bit) / 8] >> ((__bit) % 8)) & 1)

#define DIV_ROUNDUP(A, B) ({ \
    __typeof__(A) _a_ = A;       \
    __typeof__(B) _b_ = B;       \
    (_a_ + (_b_ - 1)) / _b_; \
})

size_t bitmap_size;

static uintptr_t highest_page = 0;

uint8_t *bitmap;

void *memset(void *bufptr, int value, size_t size)
{
    unsigned char *buf = (unsigned char *)bufptr;
    size_t i;
    for (i = 0; i < size; i++)
        buf[i] = (unsigned char)value;

    return bufptr;
}

void PMM_free_page(void *address)
{
    uint32_t index = (uint32_t)(uintptr_t)address / PAGE_SIZE;
    BIT_CLEAR(index);
}

void PMM_reserve_page(void *address)
{
    uint32_t index = (uint32_t)(uintptr_t)address / PAGE_SIZE;
    BIT_SET(index);
}

void PMM_free_pages(void *address, uint32_t page_count)
{
    uint32_t i;
    for (i = 0; i < page_count; i++)
    {
        PMM_free_page((void *)(address + (i * PAGE_SIZE)));
    }
}

void PMM_reserve_pages(void *address, uint32_t page_count)
{
    uint32_t i;
    for (i = 0; i < page_count; i++)
    {
        PMM_reserve_page((void *)(address + (i * PAGE_SIZE)));
    }
}

void *PMM_allocate(uint32_t pages)
{

    uint32_t i, j;
    for (i = 0; i < bitmap_size * 8; i++)
    {
        for (j = 0; j < pages; j++)
        {
            if (BIT_TEST(i))
                break;
            else if (!BIT_TEST(i) && j == pages - 1)
            {
                PMM_reserve_pages((void *)(uintptr_t)(i * PAGE_SIZE), pages);

                log(INFO, "Returning stuff");
                return (void *)(uintptr_t)(i * PAGE_SIZE);
            }
        }
    }
    return NULL;
}

/* Allocate x pages filled with zeros */
void *PMM_allocate_zero(uint64_t pages)
{
    uint64_t *p = (uint64_t *)PMM_allocate(pages);

    uint64_t i;

    for (i = 0; i < (pages * PAGE_SIZE); i++)
        p[i] = 0;
    return (void *)p;
}

void PMM_init(struct stivale2_mmap_entry *memory_map, size_t memory_entries)
{
    module("PMM");

    log(INFO, "Memory Address: %x", (uint64_t)memory_map);

    uintptr_t top;

    int i;
    for (i = 0; (size_t)i < memory_entries; i++)
    {
        struct stivale2_mmap_entry entry = memory_map[i];

        log(INFO, "[Entry %d] [%x - %x]: size: %x, type: %d", i, entry.base, entry.base + entry.length, entry.length, entry.type);

        if (entry.type != STIVALE2_MMAP_USABLE &&
            entry.type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE &&
            entry.type != STIVALE2_MMAP_KERNEL_AND_MODULES)
            continue;

        top = entry.base + entry.length;

        if (top > highest_page)
            highest_page = top;
    }

    if (top > highest_page)
        highest_page = top;

    log(INFO, "Highest page: %x", highest_page);
    
    bitmap_size = DIV_ROUNDUP(highest_page, PAGE_SIZE) / 8;

    log(INFO, "%x", bitmap_size);
    for (i = 0; (size_t)i < memory_entries; i++) /* Find a place for the bitmap */
    {
        struct stivale2_mmap_entry entry = memory_map[i];

        if (entry.type != STIVALE2_MMAP_USABLE)
            continue;

        if (entry.length >= bitmap_size)
        {
            bitmap = (uint8_t *)entry.base;

            memset(bitmap, 0xff, bitmap_size);

            entry.base += bitmap_size;
            entry.length -= bitmap_size;

            break;
        }
    }

    size_t j;
    uintptr_t k;

    for (j = 0; j < memory_entries; j++)
    {
        if (memory_map[j].type != STIVALE2_MMAP_USABLE)
            continue;

        for (k = 0; k < memory_map[i].length; k += PAGE_SIZE)
            PMM_free_pages((void *)memory_map[j].base, memory_map[j].length / PAGE_SIZE);
    }

    module("PMM");

    log(INFO, "initialized!");
}
