/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_ALLOC_H
#define LIBEMERALD_ALLOC_H
#include <arch/arch.h>
#include <emerald/std.h>

#define DIV_ROUNDUP(A, B) (      \
    {                            \
        __typeof__(A) _a_ = A;   \
        __typeof__(B) _b_ = B;   \
        (_a_ + (_b_ - 1)) / _b_; \
    })
void *alloc_malloc(size_t size);
void alloc_free(void *ptr);
void *alloc_realloc(void *ptr, size_t size);

#endif
