/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_ALLOC_H
#define LIBEMERALD_ALLOC_H

#ifndef HOST
#include <arch/arch.h>
#endif

#include <emerald/std.h>

#ifndef HOST
#    define DIV_ROUNDUP(A, B) (      \
        {                            \
            __typeof__(A) _a_ = A;   \
            __typeof__(B) _b_ = B;   \
            (_a_ + (_b_ - 1)) / _b_; \
        })

void *alloc_malloc(size_t size);
void alloc_free(void *ptr);
void *alloc_realloc(void *p, size_t size);
void *alloc_malloc_end(size_t size);

#else
#include <stdlib.h>
static inline void *alloc_malloc(size_t size) { return malloc(size); };
static inline void alloc_free(void* ptr) { return free(ptr);};
static inline void *alloc_realloc(void* p, size_t size) { return realloc(p,size);};
#endif

#endif
