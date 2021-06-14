/*
* Header from https://github.com/cute-engineewing/mulib, modified by Abb1x
*/

#ifndef LIBEMERALD_SCAN_H
#define LIBEMERALD_SCAN_H

#include <emerald/std.h>

typedef struct
{
    char *buffer;
    size_t size, head;
} Scanner;

char scan_peek(Scanner *scan, size_t offset);

char scan_current(Scanner *scan);

void scan_forward(Scanner *scan);

void scan_forward_n(Scanner *scan, size_t n);

bool scan_ended(Scanner *scan);

bool scan_continue(Scanner *scan);

bool scan_current_is_c(Scanner *scan, char c);

bool scan_current_is_w(Scanner *scan, char *word);

bool scan_current_is_b(Scanner *scan, char *buffer, size_t size);

bool scan_any_c(Scanner *scan, char c);

bool scan_any_w(Scanner *scan, char *word);

bool scan_skip_c(Scanner *scan, char c);

#endif
