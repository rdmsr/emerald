global fb_font

section .data
align 8
fb_font:
	incbin "../libraries/libk/graphics/fb_font.psf"
