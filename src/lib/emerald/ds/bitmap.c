/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/ds/bitmap.h>

void bitmap_clear(Bitmap *self, size_t index)
{
    uint64_t bit = index % 8;
    uint64_t byte = index / 8;

    self->data[byte] &= ~(1 << bit);
}

void bitmap_set(Bitmap *self, size_t index)
{
    uint64_t bit = index % 8;
    uint64_t byte = index / 8;

    self->data[byte] |= (1 << bit);
}

bool bitmap_get(Bitmap *self, size_t index)
{
    uint64_t bit = index % 8;
    uint64_t byte = index / 8;

    return self->data[byte] & (1 << bit);
}

void bitmap_fill(Bitmap *self, bool value)
{
    if (value)
    {
        mem_set(self->data, 0xff, self->size);
    }
    else
    {
        mem_set(self->data, 0, self->size);
    }
}
