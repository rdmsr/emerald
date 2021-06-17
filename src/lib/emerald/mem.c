/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/mem.h>

void *memset(void *s, int c, unsigned int len)
{
    uint8_t *s_uint8 = (uint8_t *)s;
    size_t i;
    for (i = 0; i < len; i++)
    {
        s_uint8[i] = (uint8_t)c;
    }
    return s;
}
