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

#ifndef VMM_H
#define VMM_H

#include <libk/logging.h>
#include <stdbool.h>
#include <stdint.h>
#define KERNEL_OFFSET 0xffffffff80000000

#define VIRT_TO_PHYS(a) ((uint64_t)(a)-MEM_OFFSET)
#define PHYS_TO_VIRT(a) ((uint64_t)(a) + MEM_OFFSET)

typedef struct
{
    uintptr_t *top_level;
} Pagemap;

void VMM_init(void);
void VMM_switch_pagemap(Pagemap *map);
void VMM_map_page(Pagemap *pagemap, uintptr_t physical_address, uint64_t virtual_address, uintptr_t flags);

Pagemap *VMM_new_pagemap(void);
#endif
