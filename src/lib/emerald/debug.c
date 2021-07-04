/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/debug.h>
#include <emerald/io/print.h>
#include <arch/arch.h>

void __kassert(const char *expr, const char *filename, int line)
{
  print(arch_debug_writer(), "\033[31m{a}:{i}\033[0m assertion failed ({a})\n", filename, line, expr);
}
