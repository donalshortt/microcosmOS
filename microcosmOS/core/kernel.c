#include "../util/multiboot2.h"
#include "../mm/pmm/pmm.h"
#include "../mm/vmm/vmm.h"
#include "../mm/mm.h"
#include "../front/front.h"
#include "../util/types.h"
#include "../test/test.h"

int kernel_main(unsigned long boot_info_addr)
{
    
    //TODO: Remove this magic number
    if (boot_info_addr & 7) {
        print_string("Incorrect alignment for multiboot info", 0, VGA_RED);
        goto panic;
    }

    print_name();

    setup_pmm(boot_info_addr);

	void* addr = pmm_alloc_block();
	vmm_map_page((uint64_t)addr, MAIN_MEMORY_START);

	void* value = (void*)MAIN_MEMORY_START;
	//value* = 0x1;
	int i = 0;

	run_tests();

    while(1);
    
    panic:
    return 1;
}
