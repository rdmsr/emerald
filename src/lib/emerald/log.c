/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/log.h>

String get_color(LogLevel level)
{
    switch (level)
    {
    case INFO:
        return make_str("\033[1;32m");
    case PANIC:
    case ERROR:
        return make_str("\033[1;31m");
    case TODO:
        return make_str("\033[1;35m");
    case DEBUG:
        return make_str("\033[1;36m");
    case WARNING:
        return make_str("\033[1;33m");
    }
    return make_str("");
}

String get_prefix(LogLevel level)
{
    switch (level)
    {
    case INFO:
        return make_str("INFO");
    case ERROR:
        return make_str("ERROR");
    case DEBUG:
        return make_str("DEBUG");
    case WARNING:
        return make_str("WARNING");
    case PANIC:
        return make_str("PANIC");
    case TODO:
        return make_str("TODO");
    default:
        return make_str("");
    }

    return make_str("");
}

void __log(LogLevel level, int line, char *file, char *format, ...)
{
    print(arch_debug_writer(), "{s}{s} \033[0m", get_color(level), get_prefix(level));
    print(arch_debug_writer(), "\033[30;2m{a}:{i} \033[0m\033[0m", file, line);

    va_list args;
    char buf[1024] = {0};
    va_start(args, format);

    fmt_buffer(buf, format, args);
    arch_debug_writer()->write(buf);

    va_end(args);

    print(arch_debug_writer(), "\n", file, line);
}
