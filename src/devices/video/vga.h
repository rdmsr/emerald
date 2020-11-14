#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT* COLUMNS_IN_LINE* LINES
#define LINES 25
extern void kprint(const char* str, int color);
extern void kprint_newline(void);
extern void kprint_load(char* str, bool error);
extern void clear_screen(void);
