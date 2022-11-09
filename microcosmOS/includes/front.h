#ifndef __FRONT_H_
#define __FRONT_H_

#define VIDEO_START 0xb8000
#define VGA_GREEN 2
#define VGA_RED 4
#define VGA_LIGHT_GRAY 7

void print_string(const char *str, int line, int colour);

void print_name(void);
void print_phys(void);

#endif
