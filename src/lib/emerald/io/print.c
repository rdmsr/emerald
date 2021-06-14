/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <devices/com.h>
#include <emerald/io/print.h>
#include <emerald/str/fmt.h>

void print(Writer *writer, char *str,...)
{
    va_list args;
    va_start(args, str);
    char buf[1024] = {0};

    fmt_buffer(buf, str, args);

    io_write(writer, buf);
    va_end(args);
}
