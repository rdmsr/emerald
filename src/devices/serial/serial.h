#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#ifndef SERIAL_H
#define SERIAL_H
void EmeraldDevices_Serial_init_serial();
int EmeraldDevices_Serial_serial_received();
char EmeraldDevices_Serial_read_serial();
int EmeraldDevices_Serial_is_transmit_empty();
void EmeraldDevices_Serial_write_serial(char a);
void puts(char* string);
void putchar(char character);
#endif
