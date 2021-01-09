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
#include "boot.h"
#include <debug-utilities/logger.h>
#include <inc/stivale2.h>
uintptr_t convert_to_mb(uintptr_t bytes)
{
    uintptr_t megabytes = bytes / 1024 / 1024;
    return megabytes;
}
boot_info *EmeraldBoot_Stivale_get_boot_info(struct stivale2_struct *info)
{
    struct stivale2_tag *tag = (struct stivale2_tag *)info->tags;
    struct stivale2_struct_tag_framebuffer *videoheader = videoheader;
    struct stivale2_struct_tag_memmap *memory_map = memory_map;
    boot_info *bootinfo = bootinfo;
    uint64_t usable_mem = 0;
    uint64_t total_mem = 0;
    while (tag != NULL)
    {
        switch (tag->identifier)
        {

        case STIVALE2_STRUCT_TAG_MEMMAP_ID:
        {
            memory_map = (struct stivale2_struct_tag_memmap *)tag;
            for (size_t i = 0; i < memory_map->entries; i++)
            {
                struct stivale2_mmap_entry *entry = &memory_map->memmap[i];
                total_mem += entry->length;
                if (entry->type == STIVALE2_MMAP_USABLE)
                    usable_mem += entry->length;
            }
            bootinfo->memory_usable = usable_mem;
	    bootinfo->total_memory = total_mem;
            break;
        }
        }

        tag = (struct stivale2_tag *)tag->next;
    }
    log(INFO, "Memory Size is %d mb", convert_to_mb(bootinfo->total_memory) + 1);
    return bootinfo;
}
