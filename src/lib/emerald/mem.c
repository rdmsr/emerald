/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/mem.h>

void *mem_set(void *s, int c, size_t len)
{
    uint8_t *p = (uint8_t *)s;

    size_t i;
    
    for (i = 0; i < len; i++)
    {
        p[i] = (uint8_t)c;
    }

    return s;
}

Range range_align(Range range, size_t alignment)
{
    range.base &= ~(alignment - 1);
    range.length = (range.length + (alignment - 1)) & ~(alignment - 1);

    return range;
}

void mem_cpy(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    size_t i;
    for (i = 0; i < n; i++)
    {
        pdest[i] = psrc[i];
    }
}

void mem_move(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    size_t i;
    if (src > dest)
    {
        for (i = 0; i < n; i++)
        {
            pdest[i] = psrc[i];
        }
    }
    else if (src < dest)
    {
        for (i = n; i > 0; i--)
        {
            pdest[i - 1] = psrc[i - 1];
        }
    }
}
