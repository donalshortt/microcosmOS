#include "../includes/multiboot2.h"
#include "../includes/pmm.h"
#include "../includes/vmm.h"
#include "../includes/front.h"
#include "../includes/types.h"
#include "../includes/test.h"

int kernel_main(unsigned long boot_info_addr)
{
    
    //TODO: Remove this magic number
    if (boot_info_addr & 7) {
        print_string("Incorrect alignment for multiboot info", 0, VGA_RED);
        goto panic;
    }

    print_name();

    setup_pmm(boot_info_addr);


    void* virt = (void*)0xFFFFFF0000000000;
    void* phys = pmm_alloc_block();
    vmm_map_page(virt, phys);

	run_tests();

    while(1);
    
    panic:
    return 1;
}
