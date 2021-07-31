/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_FMT_H
#define LIBEMERALD_FMT_H

#include <emerald/io/scan.h>
#include <emerald/str/str.h>
#include <emerald/std.h>

void fmt_buffer(char* buffer, char* string, va_list args);

String fmt_str(char* buf, char* string, ...);

#endif
