#define VIDEO_START 0xb8000
#define VGA_LIGHT_GRAY 7
#define VGA_RED 4

#include "includes/multiboot2.h"

static void print_string(const char *str, int line, int colour)
{
    unsigned char *video = ((unsigned char *)VIDEO_START);
    video += line * 160;

    while(*str != '\0') {
        *(video++) = *str++;
        *(video++) = colour;
    }
}

static void print_name(void)
{
    print_string("              _                                           ____  _____", 0, VGA_LIGHT_GRAY);
    print_string("   ____ ___  (_)_____________  _________  _________ ___  / __ \\/ ___/", 1, VGA_LIGHT_GRAY);
    print_string("  / __ `__ \\/ / ___/ ___/ __ \\/ ___/ __ \\/ ___/ __ `__ \\/ / / /\\__ \\ ", 2, VGA_LIGHT_GRAY);
    print_string(" / / / / / / / /__/ /  / /_/ / /__/ /_/ (__  ) / / / / / /_/ /___/ / ", 3, VGA_LIGHT_GRAY);
    print_string("/_/ /_/ /_/_/\\___/_/   \\____/\\___/\\____/____/_/ /_/ /_/\\____//____/  ", 4, VGA_LIGHT_GRAY);
}




int kernel_main(unsigned long boot_info_addr)
{
    
    //TODO: Remove this magic number
    if (boot_info_addr & 7) {
        print_string("Incorrect alignment for multiboot info", 0, VGA_RED);
        goto panic;
    }

    print_name();

    print_string("Wow", 9, VGA_LIGHT_GRAY);

    get_total_mem(boot_info_addr);

    while(1);
    
    panic:
    return 1;
}
