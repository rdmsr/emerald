#include "kbutils.h"

#include "keyboard_map.h"
void EmeraldDevices_keyboard_Keyboard_init(void)
{

	outb(0x21 , 0xFD);
}
void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
 
	outb(PIC1_COMMAND,PIC_EOI);
}
void EmeraldDevices_keyboard_Keyboard_handler_main(void *nothing)
{
	unsigned char status;
	char keycode;

	outb(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	keycode = read_port(KEYBOARD_DATA_PORT);
	if(keycode < 0){
		PIC_sendEOI(0);
		return;
		}

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
