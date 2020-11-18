#include "vga.h"
#include "../../serial/serial.h"
//TODO: Switch to a better video driver (bootloader framebuffer)
unsigned int current_loc = 0;
char* vidptr = (char*)0xb8000;
static unsigned char EmeraldASM_inb(unsigned short port)
{
    unsigned char ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}
static inline void EmeraldASM_outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port)
                 :);
}
void kprint(const char* str, int color)
{
    unsigned int i = 0;
    while (str[i] != '\0') {
        vidptr[current_loc++] = str[i++];
        vidptr[current_loc++] = color;
    }

}
void kprint_newline(void)
{
    unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
    current_loc = current_loc + (line_size - current_loc % (line_size));
}
void kprint_load(char* str, bool error)
{
    if (error == false) {

        kprint("[ ", 7);
        kprint("OK ", 10);
        kprint("] ", 7);
        kprint(str, 0x07);
        kprint(" loaded", 0x07);
        kprint_newline();
    } else if (error == true) {
        kprint("[ ", 7);
        kprint("ERROR ", 12);
        kprint("] ", 7);
        kprint(str, 0x07);
        kprint(" failed to load", 0x07);
        kprint_newline();
    }
}

void clear_screen(void)
{
    unsigned int i = 0;
    while (i < SCREENSIZE) {
        vidptr[i++] = ' ';
        vidptr[i++] = 0x07;
    }
}
void EmeraldDevices_VGA_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	EmeraldASM_outb(0x3D4, 0x0A);
	EmeraldASM_outb(0x3D5, (EmeraldASM_inb(0x3D5) & 0xC0) | cursor_start);
 
	EmeraldASM_outb(0x3D4, 0x0B);
	EmeraldASM_outb(0x3D5, (EmeraldASM_inb(0x3D5) & 0xE0) | cursor_end);
}
void EmeraldDevices_VGA_update_cursor(int x, int y)
{
	uint16_t pos = y * 80 + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}