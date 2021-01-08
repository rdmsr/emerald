#ifndef VBE_H
#define VBE_H
#pragma once
#include <inc/stivale2.h>
#include <stddef.h>
#define RED_SHIFT   16
#define GREEN_SHIFT 8
#define BLUE_SHIFT  0

typedef struct
{
    uint8_t r, g, b;
} color_t;
void EmeraldDevices_VBE_init(struct stivale2_struct *info);
void EmeraldDevices_VBE_draw_pixel(size_t x, size_t y, uint32_t color);
void EmeraldDevices_VBE_clear_screen();
void EmeraldDevices_VBE_putchar(char character, int position_x, int position_y,color_t color);
void EmeraldDevices_VBE_print(char* string,color_t color);
void EmeraldDevices_VBE_put(char c,color_t color);
#endif
