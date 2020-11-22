#include "logger.h"
#include <devices/serial/serial.h>
#include <libstr/string.h>
#include <stdarg.h>


void log(char* format, ...)
{
    EmeraldDevices_RTC_read_rtc();
    printf("%d:%d:%d [\033[1;33m LOG \033[1;33m\033[0m] %s",hour,minute,second," ");

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

    EmeraldDevices_Serial_write_serial(0x0A);
}
