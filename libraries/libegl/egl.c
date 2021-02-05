/* --------------------------- */
/* Emerald Graphics Library    */
/* Made by PQCraft 2021 - 2021 */
/* --------------------------- */

#include "egl.h"
#include <devices/video/vbe.h>
#include <boot/stivale2.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <ascii.h>

char EGL_VER[] = "1.0";

int intAbs(int num)
{
    if (num < 0) {
        return -num;
    } else {
        return num;
    }
}

uint32_t EGL_randColor()
{
    return (uint32_t)((randByte() << 24) | (randByte() << 16) | (randByte() << 8) | (randByte() << 0));
}

void EGL_drawPixel(int x, int y, uint32_t c)
{
    position_t tmpPos = {x, y};
    if (tmpPos.x < fb_info->framebuffer_width && tmpPos.y < fb_info->framebuffer_height) {VBE_draw_pixel(tmpPos, c);}
}

void EGL_drawLine(int x0, int y0, int x1, int y1, uint32_t c)
{
    bool steep = intAbs(y1 - y0) > intAbs(x1 - x0);
    if (steep) {
        int tmpSwap = x0;
        x0 = y0;
        y0 = tmpSwap;
        tmpSwap = x1;
        x1 = y1;
        y1 = tmpSwap;
    }
    if (x0 > x1) {
        int tmpSwap = x0;
        x0 = x1;
        x1 = tmpSwap;
        tmpSwap = y0;
        y0 = y1;
        y1 = tmpSwap;
    }
    int dx = x1 - x0, dy = intAbs(y1 - y0), ce = dx / 2, y = y0;
    int ys;
    if (y0 < y1) {
        ys = 1;
    } else {
        ys = -1;
    }
    int x = x0;
    int i;
    for (i = 0; i <= x1 - x0; i++) {
        if (steep) {
            EGL_drawPixel(y, x, c);
        } else {
            EGL_drawPixel(x, y, c);
        }
        ce -= dy;
        if (ce < 0) {
            y += ys;
            ce += dx;
        }
        x++;
    }
}

void EGL_drawRectangle(int x1, int y1, int x2, int y2, uint32_t color)
{
    EGL_drawLine(x1, y1, x2, y1, color);
    EGL_drawLine(x2, y1, x2, y2, color);
    EGL_drawLine(x1, y1, x1, y2, color);
    EGL_drawLine(x1, y2, x2, y2, color);
}

void EGL_drawFRectangle(int x1, int y1, int x2, int y2, uint32_t color)
{
    if (y2 < y1) {
        int tmpSwap = y1;
        y1 = y2;
        y2 = tmpSwap;
    }
    int i;
    for (i = y1; i <= y2; i++) {
        EGL_drawLine(x1, i, x2, i, color);
    }
}

void EGL_drawCircle(int xm, int ym, int r, uint32_t color)
{
    int x = -r, y = 0, err = 2 - 2 * r;
    do {
        EGL_drawPixel(xm - x, ym + y, color);
        EGL_drawPixel(xm - y, ym - x, color);
        EGL_drawPixel(xm + x, ym - y, color);
        EGL_drawPixel(xm + y, ym + x, color);
        r = err;
        if (r >  x) err += ++x * 2 + 1;
        if (r <= y) err += ++y * 2 + 1;
    } while (x < 0);
}

void EGL_drawFCircle(int x0, int y0, int radius, uint32_t c)
{
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        int i;
        for (i = x0 - x; i <= x0 + x; i++)
        {
            EGL_drawPixel(i, y0 + y, c);
            EGL_drawPixel(i, y0 - y, c);
        }
        for (i = x0 - y; i <= x0 + y; i++)
        {
            EGL_drawPixel(i, y0 + x, c);
            EGL_drawPixel(i, y0 - x, c);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

void EGL_drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    EGL_drawLine(x1, y1, x2, y2, color);
    EGL_drawLine(x2, y2, x3, y3, color);
    EGL_drawLine(x3, y3, x1, y1, color);
}

void dFT(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t c)
{
    bool steep = intAbs(y1 - y0) > intAbs(x1 - x0);
    if (steep) {
        int tmpSwap = x0;
        x0 = y0;
        y0 = tmpSwap;
        tmpSwap = x1;
        x1 = y1;
        y1 = tmpSwap;
    }
    if (x0 > x1) {
        int tmpSwap = x0;
        x0 = x1;
        x1 = tmpSwap;
        tmpSwap = y0;
        y0 = y1;
        y1 = tmpSwap;
    }
    int dx = x1 - x0, dy = intAbs(y1 - y0), ce = dx / 2, y = y0;
    int ys;
    if (y0 < y1) {
        ys = 1;
    } else {
        ys = -1;
    }
    int x = x0;
    int i;
    for (i = 0; i <= x1 - x0; i++) {
        if (steep) {
            EGL_drawLine(y, x, x2, y2, c);
        } else {
            EGL_drawLine(x, y, x2, y2, c);
        }
        ce -= dy;
        if (ce < 0) {
            y += ys;
            ce += dx;
        }
        x++;
    }
}

void EGL_drawFTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    dFT(x0, y0, x1, y1, x2, y2, color);
    dFT(x1, y1, x2, y2, x0, y0, color);
    dFT(x2, y2, x0, y0, x1, y1, color);
}

void EGL_randFill(int x1, int y1, int x2, int y2)
{
    int i, j;
    position_t position;
    for (i = y1; i <= y2; i++)
    {
        for (j = x1; j <= x2; j++)
        {
            position.x = j;
            position.y = i;
            uint32_t c = EGL_randColor();
            VBE_draw_pixel(position, c);
            while (VBE_read_pixel(position) != c) {VBE_draw_pixel(position, c);}
        }
    }
}


