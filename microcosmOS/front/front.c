#include "../includes/front.h"

void print_string(const char *str, int line, int colour)
{
    unsigned char *video = ((unsigned char *)VIDEO_START);
    video += line * 160;

    while(*str != '\0') {
        *(video++) = *str++;
        *(video++) = colour;
    }
}

void print_phys()
{

}

void print_name(void)
{
    print_string("              _                                           ____  _____", 0, VGA_LIGHT_GRAY);
    print_string("   ____ ___  (_)_____________  _________  _________ ___  / __ \\/ ___/", 1, VGA_LIGHT_GRAY);
    print_string("  / __ `__ \\/ / ___/ ___/ __ \\/ ___/ __ \\/ ___/ __ `__ \\/ / / /\\__ \\ ", 2, VGA_LIGHT_GRAY);
    print_string(" / / / / / / / /__/ /  / /_/ / /__/ /_/ (__  ) / / / / / /_/ /___/ / ", 3, VGA_LIGHT_GRAY);
    print_string("/_/ /_/ /_/_/\\___/_/   \\____/\\___/\\____/____/_/ /_/ /_/\\____//____/  ", 4, VGA_LIGHT_GRAY);
}
