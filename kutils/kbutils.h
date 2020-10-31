#include <stdint.h>
#include <stdarg.h>
#define ENTER_KEY_CODE 0x1C
#define DELETE_KEY_CODE 0x08
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES
#define LINES 25
extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
unsigned int current_loc = 0;
extern char read_port(unsigned short port);
char *vidptr = (char*)0xb8000;
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("out %0, %1" : : "a"(value), "Nd"(port) :);
}
void kb_init(void)
{

	outb(0x21 , 0xFD);
}

__attribute__((interrupt)) 
void keyboard_handler_main(void *nothing)
{
	unsigned char status;
	char keycode;

	outb(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;

		if(keycode == ENTER_KEY_CODE) {
			kprint_newline();
			return;
		}
		if(keycode == DELETE_KEY_CODE) {
			kprint_newline();
			return;
		}

		vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = 0x07;
	}
}
