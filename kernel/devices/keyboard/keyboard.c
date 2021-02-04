/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2020 Abb1x
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "keyboard.h"
#include <devices/video/vbe.h>
#include <libk/io.h>
#include <libk/logging.h>
#include <boot/stivale2.h>
#define true 1
#define false 0
#define bool uint8_t

bool shift = false;
bool ctrl = false;
bool alt = false;
bool s_lock = false;
bool n_lock = false;
bool c_lock = false;
bool kbKey[128] = {0};
uint8_t lastKey = 0;
uint8_t c;
bool rsk = false;

bool xor(uint8_t bool1, uint8_t bool2) {return (bool1 == true || bool2 == true) && (bool1 != bool2);}

void rfKbLeds() {
    IO_outb(0x60, 0xED);
    IO_outb(0x60, (s_lock * 1) + (n_lock * 2) + (c_lock * 4));
    return;
}

void setKbLeds(bool tn_lock, bool tc_lock, bool ts_lock) {
    s_lock = ts_lock;
    n_lock = tn_lock;
    c_lock = tc_lock;
    rfKbLeds();
    return;
}

void Keyboard_init(void)
{
    module("Keyboard");
    IO_outb(0x60, 0xF4);
    IO_outb(0x60, 0xF0);
    IO_outb(0x60, 0x03);
    IO_outb(0x60, 0xF7);
    /*IO_outb(0x60, 0xFD);
    IO_outb(0x60, 0xF0);
    IO_outb(0x60, 0x03);
    IO_outb(0x60, 0xF7);*/
    log(INFO, "Keyboard initialized!");
}

uint8_t getKeyCode() {
    if (c == 42 || c == 54) {shift = true;}
    if (c == 170 || c == 182) {shift = false;}
    if (c == 56 || c == 58) {ctrl = true;}
    if (c == 184 || c == 186) {ctrl = false;}
    if (c == 113 || c == 114) {alt = true;}
    if (c == 241 || c == 242) {alt = false;}
    if (c == 29) {c_lock = !c_lock; /*rfKbLeds();*/}
    if (c > 127) {
        c -= 128;
        kbKey[c] = false;
        c += 128;
    } else {
        kbKey[c] = true;
    }
    return c;
}

unsigned char getKeyAsc(unsigned char keyCode) {
    switch (keyCode) {
        case 100: return 27;
        case 2: if (shift) {return 33;} else {return 49;}
        case 3: if (shift) {return 64;} else {return 50;}
        case 4: if (shift) {return 35;} else {return 51;}
        case 5: if (shift) {return 36;} else {return 52;}
        case 6: if (shift) {return 37;} else {return 53;}
        case 7: if (shift) {return 94;} else {return 54;}
        case 8: if (shift) {return 38;} else {return 55;}
        case 9: if (shift) {return 42;} else {return 56;}
        case 10: if (shift) {return 40;} else {return 57;}
        case 11: if (shift) {return 41;} else {return 48;}
        case 12: if (shift) {return 95;} else {return 45;}
        case 13: if (shift) {return 43;} else {return 61;}
        case 14: return '\b';
        case 15: return 9;
        case 16: if (xor(shift, c_lock)) {return 81;} else {return 113;}
        case 17: if (xor(shift, c_lock)) {return 87;} else {return 119;}
        case 18: if (xor(shift, c_lock)) {return 69;} else {return 101;}
        case 19: if (xor(shift, c_lock)) {return 82;} else {return 114;}
        case 20: if (xor(shift, c_lock)) {return 84;} else {return 116;}
        case 21: if (xor(shift, c_lock)) {return 89;} else {return 121;}
        case 22: if (xor(shift, c_lock)) {return 85;} else {return 117;}
        case 23: if (xor(shift, c_lock)) {return 73;} else {return 105;}
        case 24: if (xor(shift, c_lock)) {return 79;} else {return 111;}
        case 25: if (xor(shift, c_lock)) {return 80;} else {return 112;}
        case 26: if (xor(shift, c_lock)) {return 123;} else {return 91;}
        case 27: if (xor(shift, c_lock)) {return 125;} else {return 93;}
        case 28: return '\n';
        case 30: if (xor(shift, c_lock)) {return 65;} else {return 97;}
        case 31: if (xor(shift, c_lock)) {return 83;} else {return 115;}
        case 32: if (xor(shift, c_lock)) {return 68;} else {return 100;}
        case 33: if (xor(shift, c_lock)) {return 70;} else {return 102;}
        case 34: if (xor(shift, c_lock)) {return 71;} else {return 103;}
        case 35: if (xor(shift, c_lock)) {return 72;} else {return 104;}
        case 36: if (xor(shift, c_lock)) {return 74;} else {return 106;}
        case 37: if (xor(shift, c_lock)) {return 75;} else {return 107;}
        case 38: if (xor(shift, c_lock)) {return 76;} else {return 108;}
        case 39: if (shift) {return 58;} else {return 59;}
        case 40: if (shift) {return 34;} else {return 39;}
        case 41: if (shift) {return 126;} else {return 96;}
        case 117: if (shift) {return 124;} else {return 92;}
        case 44: if (xor(shift, c_lock)) {return 90;} else {return 122;}
        case 45: if (xor(shift, c_lock)) {return 88;} else {return 120;}
        case 46: if (xor(shift, c_lock)) {return 67;} else {return 99;}
        case 47: if (xor(shift, c_lock)) {return 86;} else {return 118;}
        case 48: if (xor(shift, c_lock)) {return 66;} else {return 98;}
        case 49: if (xor(shift, c_lock)) {return 78;} else {return 110;}
        case 50: if (xor(shift, c_lock)) {return 77;} else {return 109;}
        case 51: if (shift) {return 60;} else {return 44;}
        case 52: if (shift) {return 62;} else {return 46;}
        case 53: if (shift) {return 63;} else {return 47;}
        case 57: return 32;
        case 83: return 127;

        default: return 0;
    }
    return 0;
}

unsigned char getKey() {
    IO_inb(0x60);
    unsigned char c = getKeyAsc(getKeyCode());
    if (rsk) {
        if (ctrl && c > 96 && c < 123) {c -= 96;}
        uint8_t tc = c;
        /*c = 0;*/
        rsk = false;
        return tc;
    } else {return 0;}
}

void Keyboard_main()
{
    /*IO_inb(KEYBOARD_DATA_PORT);*/

    c = IO_inb(0x60);

    rsk = true;

    /*IO_outb(0x20, 0x20);*/
}
