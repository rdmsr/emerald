/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_MACROS_H
#define LIBEMERALD_MACROS_H

#define UNUSED(x) (void)x
#define ARRLEN(arr) sizeof(arr) / sizeof(arr[0])
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MAYBE_UNUSED __attribute__((unused))
#define ALIGN_DOWN(__addr, __align) ((__addr) & ~((__align)-1))
#define ALIGN_UP(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))

#define PACKED __attribute__((packed))

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#endif
