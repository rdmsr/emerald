/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/mem.h>

void *memset(void *s, int c, unsigned int len)
{
    unsigned char *p = s;
    while (len--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}


