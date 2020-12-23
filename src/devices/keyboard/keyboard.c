#include "keyboard.h"
#include <debug-utilities/logger.h>
#include <devices/RTC/rtc.h>
#include <libasm/asm.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <libint/int.h>
#define ENTER_KEY_CODE 0x1C
unsigned char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', ' ', /* Backspace */
    '\t', /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0, /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`', 0, /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 49 */
    'm', ',', '.', '/', 0, /* Right shift */
    '*',
    0, /* Alt */
    ' ', /* Space bar */
    0, /* Caps lock */
    0, /* 59 - F1 key ... > */
    0, 0, 0, 0, 0, 0, 0, 0,
    0, /* < ... F10 */
    0, /* 69 - Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up Arrow */
    0, /* Page Up */
    '-',
    0, /* Left Arrow */
    0,
    0, /* Right Arrow */
    '+',
    0, /* 79 - End key*/
    0, /* Down Arrow */
    0, /* Page Down */
    0, /* Insert Key */
    0, /* Delete Key */
    0, 0, 0,
    0, /* F11 Key */
    0, /* F12 Key */
    0, /* All other keys are undefined */
};
unsigned int current_location = 0;

void EmeraldDevices_keyboard_Keyboard_init(void)
{
    EmeraldASM_outb(0x21, 0xFD);
}
void PIC_sendEOI(unsigned char irq)
{
    if (irq >= 8)
        EmeraldASM_outb(PIC2_COMMAND, PIC_EOI);

    EmeraldASM_outb(PIC1_COMMAND, PIC_EOI);
}
void EmeraldDevices_keyboard_Keyboard_handler_main()
{
    char keycode;
    EmeraldASM_outb(0x20, 0x20);
    EmeraldASM_inb(KEYBOARD_STATUS_PORT);
    keycode = EmeraldASM_inb(KEYBOARD_DATA_PORT);
    if (keycode < 0) {
        PIC_sendEOI(0);
        return;
    }

    if (keycode == ENTER_KEY_CODE) {
        kprint_newline();
        return;
    }
    if (keycode == DELETE_KEY_CODE) {
        kprint_newline();
        return;
    }
    /*log("Interrupt pressed: %d letter: %c",keycode,keyboard_map[(unsigned char)keycode]);*/
    vidptr[current_location++] = keyboard_map[(unsigned char)keycode];
    vidptr[current_location++] = 0x07;
    EmeraldDevices_VGA_update_cursor(current_location / 2, 0);
}
