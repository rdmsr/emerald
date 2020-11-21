#include "string.h"
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

