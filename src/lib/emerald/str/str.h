/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_STR_H
#define LIBEMERALD_STR_H
#include <emerald/std.h>

typedef struct
{
    char *buffer;
    size_t size;
} String;

#define string_fixed(length) \
    struct                   \
    {                        \
        char buffer[length]; \
        size_t size;         \
    }                        \

typedef string_fixed(8) StringFixed8;
typedef string_fixed(16) StringFixed16;
typedef string_fixed(32) StringFixed32;
typedef string_fixed(64) StringFixed64;
typedef string_fixed(128) StringFixed128;

size_t cstrlen(char *str);
String str_concat(String str, String str2);
String make_str(char *str);
String str_convert(unsigned int num, int base);
#endif
