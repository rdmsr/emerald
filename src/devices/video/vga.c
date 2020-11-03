#include "vga.h"
unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;
void kprint(const char *str,int color)
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
void kprint_load(char *str,bool error){
	if(error == false){

	kprint("[ ",7);
	kprint("OK ",10);
	kprint("] ",7);
	kprint(str,0x07);
	kprint(" loaded",0x07);
	kprint_newline();
}
	else if (error == true)
	{
	kprint("[ ",7);
	kprint("ERROR ",12);
	kprint("] ",7);
	kprint(str,0x07);
	kprint(" failed to load",0x07);
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
