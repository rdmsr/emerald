/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_MAYBE_H
#define LIBEMERALD_MAYBE_H

#include <emerald/std.h>
#include <emerald/str.h>

#define new_maybe(T) \
    typedef struct   \
    {                \
        bool ok;     \
        T just;      \
    } __maybe_##T

new_maybe(int);
new_maybe(char);
new_maybe(String);
new_maybe(u64);
new_maybe(u16);
new_maybe(u32);

#define Just(T, val)   \
    (__maybe_##T)      \
    {                  \
        .ok = true,    \
        .just = (val), \
    }

#define Nothing(T)   \
    (__maybe_##T)    \
    {                \
        .ok = false, \
    }

#define Maybe$(expr) (                                                                 \
    {                                                                                  \
        auto res = expr;                                                               \
        if (!res.ok)                                                                   \
        {                                                                              \
            log_panic("Value is nothing at line {}, file {}", __LINE__, __FILENAME__); \
        }                                                                              \
        res.just;                                                                      \
    })

#define Maybe(T) __maybe_##T

#endif
