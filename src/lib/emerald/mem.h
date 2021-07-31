/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_MEM_H
#define LIBEMERALD_MEM_H

#include <emerald/std.h>

void *mem_set(void *s, int c, size_t len);
void mem_cpy(void *dest, const void *src, size_t n);
void mem_move(void *dest, const void *src, size_t n);

typedef struct
{
    uintptr_t base;
    size_t length;
} Range;

Range range_align(Range range, size_t alignment);

  
#endif
