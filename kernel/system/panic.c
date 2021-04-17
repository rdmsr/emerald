#include "panic.h"
#include <devices/video/framebuffer.h>
#include <libk/logging.h>
#include <libk/random.h>
char *comments_lol[] =
    {
        "Something happened.",
        "F.",
        "The kernel crashed successfully.",
        "oops I did a woopsie doopsie OwO.",
        "Random error just to annoy you.",
        "Keyboard not found, press F to pay respects.",
        "Greenpeace freed the mallocs",
        "Typo in the code.",
        "Excuse Me Sir, do you have a moment to talk about our Lord and Saviour?",
        "Never gonna give you up",
        "E.",
        "Error 404, kernel not found.",
        "To boot or not to boot.",
        ":(",
        "Kernel is not in the sudoers file. This incident will be reported.",
        "Error Code: INSERT_FUNNY_THING_HERE"

};

void __panic(char *file, const char function[20], int line, char *message)
{

    Framebuffer_clear();

    __asm__("cli");


    Framebuffer_puts(" \033[31m------------------------------------------------------------------------\n");
    Framebuffer_puts(" KERNEL PANIC\n\033[0m");
    glog(SILENT, " \033[30m/* %s */", comments_lol[rand() % 17]);
    glog(SILENT, " \033[35m%s\033[0m", message);
    glog(SILENT, " In %s at %s(), line %d", file, function, line);

    while (1)
        ;
}
