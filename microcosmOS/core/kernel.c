#include "../includes/multiboot2.h"
#include "../includes/pmm.h"
#include "../includes/front.h"


int kernel_main(unsigned long boot_info_addr)
{
    
    //TODO: Remove this magic number
    if (boot_info_addr & 7) {
        print_string("Incorrect alignment for multiboot info", 0, VGA_RED);
        goto panic;
    }

    print_name();

    print_string("Wow", 9, VGA_LIGHT_GRAY);

    // PMM


    //setup_pmm(pmm_state, boot_info_addr);
    
    while(1);
    
    panic:
    return 1;
}
