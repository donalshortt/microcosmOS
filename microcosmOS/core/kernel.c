#include "../util/multiboot2.h"
#include "../mm/mm.h"
#include "../mm/pmm/pmm.h"
#include "../mm/vmm/vmm.h"
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

	/*uintptr_t phys = 0x40603000;
	uintptr_t virt = 0x60000000;
	vmm_map_page(phys, virt);

	uintptr_t* ptr = (uintptr_t*)virt;
	*ptr = 42;*/

	void* ptr = kmalloc(10000);

	run_tests();

	heap_init();

	int wow = 1;
    while(1);
    
    panic:
    return 1;
}
