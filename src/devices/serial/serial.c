
#include "serial.h"
#define PORT 0x3f8 //this is COM1 (for qemu debugging)

static inline void EmeraldASM_outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port)
                 :);
}
static unsigned char EmeraldASM_inb(unsigned short port)
{
    unsigned char ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}
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
// Receiving data from serial port
int EmeraldDevices_Serial_serial_received()
{
    return EmeraldASM_inb(PORT + 5) & 1;
}
char EmeraldDevices_Serial_read_serial()
{
    while(EmeraldDevices_Serial_serial_received() == 0);
    return EmeraldASM_inb(PORT);
}

// Sending data to serial port
int EmeraldDevices_Serial_is_transmit_empty()
{
    return EmeraldASM_inb(PORT + 5) & 0x20;
}
void EmeraldDevices_Serial_write_serial(char a)
{ 
    while(EmeraldDevices_Serial_is_transmit_empty() == 0);
    EmeraldASM_outb(PORT,a);
}