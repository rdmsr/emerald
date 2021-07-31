/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/debug.h>
#include <emerald/io/print.h>
#ifndef HOST
#include <arch/arch.h>
#else
#include <stdio.h>
static inline void host_writer(char* string)
{
  puts(string);
}
static inline Writer* arch_debug_writer()
{
  static Writer stdio;
  stdio.write = host_writer;
  return &stdio;
}
#endif

void __kassert(const char *expr, const char *filename, int line)
{
  print(arch_debug_writer(), "\033[31m{a}:{i}\033[0m assertion failed ({a})\n", filename, line, expr);
}
