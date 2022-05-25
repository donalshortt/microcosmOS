#define VIDEO_START 0xb8000
#define VGA_LIGHT_GRAY 7
#define VGA_RED 4

#include "includes/multiboot2.h"
#include "includes/mem_manage.h"

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

unsigned long get_total_mem(unsigned long boot_info_addr)
{
    unsigned long long total_mem = 0;
    struct multiboot_tag* mb_tag;
    
    for (mb_tag = (struct multiboot_tag *) (boot_info_addr + 8);
            mb_tag->type != MULTIBOOT_TAG_TYPE_END;
            mb_tag = (struct multiboot_tag *) ((multiboot_uint8_t *) mb_tag 
                + ((mb_tag->size + 7) & ~7)))
    {
        if (mb_tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            multiboot_memory_map_t *mmap;

            for (mmap = ((struct multiboot_tag_mmap *) mb_tag)->entries;
                    (multiboot_uint8_t *) mmap 
                        < (multiboot_uint8_t *) mb_tag + mb_tag->size;
                    mmap = (multiboot_memory_map_t *) ((unsigned long) mmap 
                        + ((struct multiboot_tag_mmap *) mb_tag)->entry_size))
            {
                if (((struct multiboot_mmap_entry *) mmap)->type == MULTIBOOT_MEMORY_AVAILABLE)
                    total_mem += ((struct multiboot_mmap_entry *) mmap)->len;
            }
        }
    }

    return total_mem;
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
