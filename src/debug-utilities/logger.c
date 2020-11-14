#include "logger.h"
#include "../devices/serial/serial.h"
unsigned int strlen(const char *s)
{
    unsigned int count = 0;

    while(*s!='\0')
    {
        count++;
        s++;
    }
    return count;
}
void log(char* a)
{
    char* logstr = "[\033[1;33mLOG\033[1;33m\033[0m] ";
    for(int j = 0;j<strlen(logstr);j++)
    {
        char str = logstr[j];
        EmeraldDevices_Serial_write_serial((int)str);
    }
    for(int i=0;i<strlen(a);i++)
    {
        char b = a[i];
        EmeraldDevices_Serial_write_serial((int)b);
    }
    EmeraldDevices_Serial_write_serial(0x0A);
}