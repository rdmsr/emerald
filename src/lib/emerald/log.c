/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/log.h>

MAYBE_UNUSED static uint32_t lock = 0;

String get_color(LogLevel level)
{
    switch (level)
    {
    case INFO:
    case PASS:
        return make_str("\033[1;32m");
	
    case PANIC:
    case ERROR:
    case FAIL:
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
    case FAIL:
        return make_str("FAIL");
    case PASS:
        return make_str("PASS");
    default:
        return make_str("");
    }

    return make_str("");
}

void __log(LogLevel level, int line, char *file, char *format, ...)
{
#ifndef HOST
    lock_acquire(&lock);
#endif
    print(arch_debug_writer(), "{s}{s} \033[0m", get_color(level), get_prefix(level));
    print(arch_debug_writer(), "\033[30m{a}:{i} \033[0m\033[0m", file, line);

    va_list args;
    char buf[1024] = {0};
    va_start(args, format);

    fmt_buffer(buf, format, args);
    io_write(arch_debug_writer(), buf);

    va_end(args);

    print(arch_debug_writer(), "\n", file, line);

#ifndef HOST
    lock_release(&lock);
#endif
}
