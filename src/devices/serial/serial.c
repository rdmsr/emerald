
#include "serial.h"
#include <libstr/string.h>
#include <libasm/asm.h>
#define PORT 0x3f8 /*this is COM1 address (for qemu debugging)*/

/*init serial*/
void EmeraldDevices_Serial_init_serial()
{
    EmeraldASM_outb(PORT + 1, 0x00);
    EmeraldASM_outb(PORT + 3, 0x80);
    EmeraldASM_outb(PORT + 0, 0x03);
    EmeraldASM_outb(PORT + 1,0x00);
    EmeraldASM_outb(PORT + 3, 0x03);
    EmeraldASM_outb(PORT + 2,0xC7);
    EmeraldASM_outb(PORT + 4, 0x0B);
}
/* Receiving data from serial port*/
int EmeraldDevices_Serial_serial_received()
{
    return EmeraldASM_inb(PORT + 5) & 1;
}
char EmeraldDevices_Serial_read_serial()
{
    while(EmeraldDevices_Serial_serial_received() == 0);
    return EmeraldASM_inb(PORT);
}

/* Sending data to serial port*/
int EmeraldDevices_Serial_is_transmit_empty()
{
    return EmeraldASM_inb(PORT + 5) & 0x20;
}
void EmeraldDevices_Serial_write_serial(char a)
{ 
    while(EmeraldDevices_Serial_is_transmit_empty() == 0);
    EmeraldASM_outb(PORT,a);
}
/* Functions to print letters*/
void puts(char* string)
{
      for(unsigned int i=0;i<strlen(string);i++)
    {
        char output = string[i];
        EmeraldDevices_Serial_write_serial((int)output);
    }  
}
void putchar(char character)
{
    EmeraldDevices_Serial_write_serial((int)(character));
}