/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_BITMAP_H
#define LIBEMERALD_BITMAP_H

#include <emerald/std.h>
#include <emerald/mem.h>

typedef struct
{
    uint8_t *data;
    size_t size;
} Bitmap;

void bitmap_clear(Bitmap *self, size_t index);

void bitmap_set(Bitmap *self, size_t index);

bool bitmap_get(Bitmap *self, size_t index);

void bitmap_fill(Bitmap *self, bool value);

#endif
