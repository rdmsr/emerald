#include "string.h"
#include <stdarg.h>
/* Reimplementations of stdlibs*/
unsigned int strlen(const char* s)
{
    unsigned int count = 0;

    while (*s != '\0') {
        count++;
        s++;
    }
    return count;
}
char* itoa(int val, int base){

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}
char* convert(unsigned int num, int base)
{
    static char Representation[] = "0123456789ABCDEF";
    static char buffer[50];
    char* ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do {
        *--ptr = Representation[num % base];
        num /= base;
    } while (num != 0);
    return (ptr);
}

void printf(char* format, ...)
{
    char* traverse;
    unsigned int i;
    char* s;
    static const unsigned ZERO = 0;
    va_list arg;
    va_start(arg, format);

    for (traverse = format; *traverse != '\0'; traverse++) {
        while (*traverse != '%') {
            putchar(*traverse);
            traverse++;
        }

        traverse++;

        switch (*traverse) {
        case 'c':
            i = va_arg(arg, int);
            putchar(i);
            break;

        case 'd':
            i = va_arg(arg, int);
            if (i < ZERO) {
                i = -i;
                putchar('-');
            }
            puts(convert(i, 10));
            break;

        case 'o':
            i = va_arg(arg, unsigned int);
            puts(convert(i, 8));
            break;

        case 's':
            s = va_arg(arg, char*);
            puts(s);
            break;

        case 'x':
            i = va_arg(arg, unsigned int);
            puts(convert(i, 16));
            break;
        }
    }

    va_end(arg);

}