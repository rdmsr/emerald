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

#include <boot/boot.h>
#include <devices/video/vbe.h>
#include <libk/logging.h>
#include <memory/pmm.h>

uintptr_t convert_to_mb(uintptr_t bytes)
{
    uintptr_t megabytes = bytes / 1024 / 1024;
    return megabytes;
}
void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id)
{
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags + MEM_OFFSET;
    for (;;)
    {
        if (!current_tag)
        {
            return NULL;
        }

        if (current_tag->identifier == id)
        {
            return current_tag;
        }

        current_tag = (void *)current_tag->next + MEM_OFFSET;
    }
}

BootInfo Boot_get_info(struct stivale2_struct *info)
{
    module("Boot");
    struct stivale2_struct_tag_memmap *memory_map = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    struct stivale2_struct_tag_framebuffer *fb_info = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    BootInfo bootinfo;
    size_t usable_mem = 0;
    uint64_t total_mem = 0;

    bootinfo.memory_entries = memory_map->entries;
    bootinfo.memory_map = memory_map;
    size_t i;

    for (i = 0; i < memory_map->entries; i++)
    {
        struct stivale2_mmap_entry *entry = &memory_map->memmap[i];
        total_mem += entry->length;
        if (entry->type == STIVALE2_MMAP_USABLE)
        {
            usable_mem += entry->length;
            bootinfo.memory_top = entry->base + entry->length;
        }
    }
    bootinfo.memory_usable = usable_mem;
    bootinfo.total_memory = total_mem;

    bootinfo.framebuffer_width = fb_info->framebuffer_width;
    bootinfo.framebuffer_height = fb_info->framebuffer_height;

    bootinfo.framebuffer_bpp = fb_info->framebuffer_bpp;
    bootinfo.framebuffer_pitch = fb_info->framebuffer_pitch;

    VBE_putf("Memory info:");
    VBE_putf("\t Total size: %d mb", convert_to_mb(bootinfo.total_memory) + 1);
    VBE_putf("\t Usable: %d mb\n", convert_to_mb(bootinfo.memory_usable) + 1);
    log(INFO, "Memory Size is %d mb", convert_to_mb(bootinfo.total_memory) + 1);
    return bootinfo;
}
