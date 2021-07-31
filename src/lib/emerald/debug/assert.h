/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_ASSERT_H
#define LIBEMERALD_ASSERT_H

#include <emerald/std.h>

void __kassert(const char* expr, const char* filename, int line);

#define kassert(__cond) ((__cond) ? (void) 0 : __kassert(#__cond, __FILE__, __LINE__))

#endif
