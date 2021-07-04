/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <arch/memory/pmm.h>
#include <boot/boot.h>
#include <emerald/debug.h>
#include <emerald/ds/bitmap.h>

static Bitmap bitmap;
static uintptr_t highest_page = 0;

void clear_page(void *addr)
{
    bitmap_clear(&bitmap, (size_t)addr / PAGE_SIZE);
}

void set_page(void *addr)
{
    bitmap_set(&bitmap, (size_t)addr / PAGE_SIZE);
}

void pmm_free(void *addr, size_t pages)
{
    size_t i;
    for (i = 0; i < pages; i++)
    {
        clear_page((void *)(addr + (i * PAGE_SIZE)));
    }
}

void set_pages(void *addr, size_t page_count)
{
    size_t i;
    for (i = 0; i < page_count; i++)
    {
        set_page((void *)(addr + (i * PAGE_SIZE)));
    }
}

void *pmm_allocate(size_t pages)
{
    kassert(pages > 0);
    kassert(pages < bitmap.size);

    size_t i, j;
    for (i = 0; i < highest_page / PAGE_SIZE; i++)
    {
        for (j = 0; j < pages; j++)
        {
            if (bitmap_get(&bitmap, i))
            {
                break;
            }

            else if (j == pages - 1)
            {
                set_pages((void *)(i * PAGE_SIZE), pages);
                return (void *)(i * PAGE_SIZE);
            }
        }
    }

    log(ERROR, "Couldn't find a free page");
    return NULL;
}

void *pmm_allocate_zero(size_t pages)
{
    void *ret = pmm_allocate(pages);

    kassert(ret != NULL);

    memset(ret, 0, pages * PAGE_SIZE);

    return ret;
}

void print_bitmap(int n)
{
    size_t i;

    print(arch_debug_writer(), "Bitmap: ");

    for (i = 0; i < (size_t)n; i++)
    {
      print(arch_debug_writer(), "{i}", bitmap_get(&bitmap, i));
    }

    print(arch_debug_writer(), "\n");
}

static const char *get_memmap_entry_type(int type)
{
    switch (type)
    {
    case STIVALE2_MMAP_USABLE:
        return "Usable";
    case STIVALE2_MMAP_RESERVED:
        return "Reserved";
    case STIVALE2_MMAP_ACPI_RECLAIMABLE:
        return "ACPI Reclaimable";
    case STIVALE2_MMAP_ACPI_NVS:
        return "ACPI Non Volatile Storage";
    case STIVALE2_MMAP_BAD_MEMORY:
        return "Bad Memory";
    case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
        return "Bootloader Reclaimable";
    case STIVALE2_MMAP_KERNEL_AND_MODULES:
        return "Kernel And Modules";
    case STIVALE2_MMAP_FRAMEBUFFER:
        return "Framebuffer";
    default:
        return "???";
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

        log(INFO, "Entry {i}: base={x} length={x} type={a}", i, entry->base, entry->length, get_memmap_entry_type(entry->type));

        if (entry->type != STIVALE2_MMAP_USABLE &&
            entry->type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE &&
            entry->type != STIVALE2_MMAP_KERNEL_AND_MODULES)
        {
            continue;
        }

        top = entry->base + entry->length;

        if (top > highest_page)
        {
            highest_page = top;
        }
    }

    size_t bitmap_size = ALIGN_UP(ALIGN_DOWN(highest_page, PAGE_SIZE) / PAGE_SIZE / 8, PAGE_SIZE);

    bitmap.size = bitmap_size;
    
    kassert(bitmap.size > 0);

    log(INFO, "The bitmap needs to be {i} kb long", bitmap.size / 1024);

    for (j = 0; j < memory_map->entries; j++)
    {
        struct stivale2_mmap_entry *entry = &memory_map->memmap[j];

        if (entry->type == STIVALE2_MMAP_USABLE && entry->length >= bitmap_size)
        {
            bitmap.data = (uint8_t *)(entry->base + MEM_PHYS_OFFSET);
            entry->base += bitmap_size;
            entry->length -= bitmap_size;
            break;
        }
    }

    bitmap_fill(&bitmap, 0xff);

    for (k = 0; k < memory_map->entries; k++)
    {
        /* If the current entry is usable, set it free in the bitmap */
        if (memory_map->memmap[k].type == STIVALE2_MMAP_USABLE)
        {
            pmm_free((void *)memory_map->memmap[k].base, memory_map->memmap[k].length / PAGE_SIZE);
        }
    }
}
