#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "../devices/keyboard/kbutils.h"
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES
#define LINES 25

extern char read_port(unsigned short port);

extern void kprint(const char *str,int color);
extern void kprint_newline(void);
extern void kprint_load(char *str,bool error);
extern void clear_screen(void);

