/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <arch/memory/pmm.h>
#include <boot/boot.h>
#include <emerald/mem.h>

static uintptr_t highest_page = 0;
static size_t last_used_index = 0;

/* TODO: implement a bitmap data structure in libemerald */
static uint8_t *bitmap;

static void release_page(void *addr)
{
    size_t index = (size_t)(uintptr_t)addr / PAGE_SIZE;
    BIT_CLEAR(index);
}

static void reserve_page(void *adr)
{
    size_t index = (size_t)(uintptr_t)adr / PAGE_SIZE;
    BIT_SET(index);
}

static void release_pages(void *addr, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        release_page((void *)(addr + (i * PAGE_SIZE)));
    }
}

void reserve_pages(void *addr, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        reserve_page((void *)(addr + (i * PAGE_SIZE)));
    }
}

static void *inner_alloc(size_t count, size_t limit)
{
    size_t p = 0;

    size_t i;
    while (last_used_index < limit)
    {
        last_used_index++;
        if (!BIT_TEST(last_used_index))
        {
            if (++p == count)
            {
                size_t page = last_used_index - count;

                if (last_used_index == count)
                {
                    page = last_used_index / count;
                }

                for (i = page - 1; i < last_used_index; i++)
                {
                    BIT_SET(i);
                }

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

void *pmm_allocate(size_t pages)
{
    size_t l = last_used_index;
    void *ret = inner_alloc(pages, highest_page / PAGE_SIZE);
    if (ret == NULL)
    {
        last_used_index = 0;
        ret = inner_alloc(pages, l);
    }

    return ret;
}

void *pmm_allocate_zero(size_t pages)
{
    char *ret = (char *)pmm_allocate(pages);

    if (ret == NULL)
        return NULL;

    uint64_t *ptr = (uint64_t *)(ret + MEM_PHYS_OFFSET);

    size_t i;
    for (i = 0; i < pages * (PAGE_SIZE / sizeof(uint64_t)); i++)
    {
        ptr[i] = 0;
    }

    return ret;
}

void print_bitmap(void)
{
    size_t i;

    for (i = 0; i < last_used_index + 10; i++)
    {
        log(INFO, "{i}", BIT_TEST(i));
    }
}

void pmm_initialize(struct stivale2_struct *boot_info)
{
    uintptr_t top;
    size_t i, j, k;

    struct stivale2_struct_tag_memmap *memory_map = stivale2_get_tag(boot_info, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    log(INFO, "Getting memory map...");
    for (i = 0; i < memory_map->entries; i++)
    {
        struct stivale2_mmap_entry *entry = &memory_map->memmap[i];

        log(INFO, "Entry {i}: base={x} length={x} type={i}", i, entry->base, entry->length, entry->type);

        if (entry->type != STIVALE2_MMAP_USABLE &&
            entry->type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE &&
            entry->type != STIVALE2_MMAP_KERNEL_AND_MODULES)
            continue;

        top = entry->base + entry->length;

        if (top > highest_page)
        {
            highest_page = top;
        }
    }

    size_t bitmap_size = ALIGN_UP(ALIGN_DOWN(highest_page, PAGE_SIZE) / PAGE_SIZE / 8, PAGE_SIZE);

    log(INFO, "The bitmap needs to be {i} kb long", bitmap_size / 1024);

    for (j = 0; j < memory_map->entries; j++)
    {

        struct stivale2_mmap_entry *entry = &memory_map->memmap[j];

        if (entry->type != STIVALE2_MMAP_USABLE)
            continue;

        if (entry->length >= bitmap_size)
        {
            bitmap = (uint8_t *)(entry->base + MEM_PHYS_OFFSET);
            entry->base += bitmap_size;
            entry->length -= bitmap_size;
            break;
        }

        memset(bitmap, 0xff, bitmap_size);

        for (k = 0; k < memory_map->entries; k++)
        {
            if (memory_map->memmap[k].type != STIVALE2_MMAP_USABLE)
            {
                continue;
            }

            else
            {
                release_pages((void *)memory_map->memmap[k].base, memory_map->memmap[k].length / PAGE_SIZE);
            }
        }
    }
}
