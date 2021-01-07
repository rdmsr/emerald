#include "vbe.h"
#include <debug-utilities/logger.h>
#include <mem/virtual/vmm.h>
#include <inc/stivale2.h>
#include <stddef.h>
struct stivale2_struct_tag_framebuffer *fb_info;
color_t bg_color = {0,64,73};

uint32_t get_color(color_t *color)
{
    return (uint32_t)((color->r << RED_SHIFT) | (color->g << GREEN_SHIFT) | (color->b << BLUE_SHIFT));
}

void EmeraldDevices_VBE_draw_pixel(size_t x, size_t y, uint32_t color)
{
    size_t fb_i = x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;
    uint32_t *fb = (uint32_t *)fb_info->framebuffer_addr;

    fb[fb_i] = color;
}
void vbe_clear_screen()
{

    color_t *color;
    color = &bg_color;
    for (int i = 0; i < fb_info->framebuffer_width; i++)
    {
        for (int j = 0; j < fb_info->framebuffer_height; j++)
        {
            EmeraldDevices_VBE_draw_pixel(i, j, get_color(color));
        }
    }
}
void EmeraldDevices_VBE_init(struct stivale2_struct *info)
{
	struct stivale2_tag *tag = (struct stivale2_tag *)info->tags;
        struct stivale2_struct_tag_framebuffer *videoheader=videoheader;
	while (tag != NULL) {
		switch (tag->identifier) {

		case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID: {
				videoheader = (struct stivale2_struct_tag_framebuffer *)tag;
				fb_info = videoheader;
				
				break;
			}
		}

		tag = (struct stivale2_tag *)tag->next;
	}
    log(DEBUG, "found a %dx%d display", fb_info->framebuffer_width,fb_info->framebuffer_height);
}
