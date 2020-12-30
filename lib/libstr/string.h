#ifndef STRING_H
#define STRING_H
#include <devices/serial/serial.h>
#include <libint/int.h>
unsigned int strlen(const char *s);
char *itoa(int val, int base);
void printf(char *format, ...);
char *convert(unsigned int num, int base);
char *strcpy(char *destination, const char *source);
#endif
