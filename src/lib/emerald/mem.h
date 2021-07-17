/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_MEM_H
#define LIBEMERALD_MEM_H

#include <emerald/std.h>

void *memset(void *s, int c, unsigned int len);
void memcpy(void *dest, void *src, size_t n);
void memmove(void *dest, void *src, size_t n);

typedef struct
{
    uintptr_t base;
    size_t length;
} Range;

Range range_align(Range range, size_t alignment);

  
#endif
