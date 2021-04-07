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

#define DIV_ROUNDUP(A, B) ({ \
    __typeof__(A) _a_ = A;   \
    __typeof__(B) _b_ = B;   \
    (_a_ + (_b_ - 1)) / _b_; \
})
#define BIT_CLEAR(__bit) (bitmap.buffer[(__bit) / 8] &= ~(1 << ((__bit) % 8)))

Bitmap bitmap;

void *memset(void *bufptr, int value, size_t size)
{
    unsigned char *buf = (unsigned char *)bufptr;
    size_t i;
    for (i = 0; i < size; i++)
        buf[i] = (unsigned char)value;

    return bufptr;
}

void PMM_init(struct stivale2_mmap_entry *memory_map, size_t memory_entries, BootInfo bootinfo)
{
    module("PMM");

    log(INFO, "Memory Address: %x", (uint64_t)memory_map);

    size_t bitmap_size = DIV_ROUNDUP(bootinfo.memory_highest_page, PAGE_SIZE) / 8;

    size_t i;
    for (i = 0; i < memory_entries; i++)
    {
        log(INFO, "[Entry %d] [%x - %x]: size: %x, type: %d", i, memory_map[i].base, memory_map[i].base + memory_map[i].length, memory_map[i].length, memory_map[i].type);

        if (memory_map[i].type != STIVALE2_MMAP_USABLE && memory_map[i].type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE && memory_map[i].type != STIVALE2_MMAP_KERNEL_AND_MODULES)
            continue;
    }

    /* Finding a place for the bitmap */

    size_t m;

    for (m = 0; m < memory_entries; m++)
    {
        struct stivale2_mmap_entry entry = memory_map[m];

        if (entry.type != STIVALE2_MMAP_USABLE)
            continue;

        if (entry.length >= bitmap_size)
        {
            uint8_t *bitmap__ = (uint8_t *)entry.base + MEM_OFFSET;

            bitmap = _Bitmap(bitmap__, bitmap_size);

            entry.base += bitmap_size;
            entry.length -= bitmap_size;

            break;
        }
    }

    /* Populating free entries */
    size_t j;
    uintptr_t k;
    for (j = 0; j < memory_entries; j++)
    {

        if (memory_map[j].type != STIVALE2_MMAP_USABLE)
            continue;

        for (k = 0; k < memory_map[j].length; k += PAGE_SIZE)
            BIT_CLEAR((memory_map[j].base + k) / PAGE_SIZE);
    }

    module("PMM");

    log(INFO, "initialized!");
}

void *PMM_allocate(uint64_t count)
{

    uint64_t res = bitmap.allocate(count, &bitmap);

    return (void *)(res * PAGE_SIZE);
}

void *PMM_allocate_zero(uint64_t count)
{
    char *ret = (char *)PMM_allocate(count);

    if (!ret)
        return NULL;

    uint64_t *ptr = (uint64_t *)(ret + MEM_OFFSET);

    size_t i;
    for (i = 0; i < count * (PAGE_SIZE / sizeof(uint64_t)); i++)
        ptr[i] = 0;

    return ret;
}
