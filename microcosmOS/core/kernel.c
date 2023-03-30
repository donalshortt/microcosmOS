#include "../util/multiboot2.h"
#include "../mm/mm.h"
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

	uintptr_t phys = 0x600000;
	uintptr_t virt = 0x600000;

	vmm_map_page(phys, virt);

	uintptr_t* test = (uintptr_t*)0x6000000;
	*test = 42;

	int i = 0;

	//void* test = kmalloc(8193);
	//kmemset(test, 1, 8193);
	//
	//uintptr_t paddr = pmm_alloc_block();
	//vmm_map_page(paddr, MAIN_MEMORY_START);
	//long* ptr = (long*)MAIN_MEMORY_START;
	//*ptr = 42;
	//
	
	/*struct myStruct {
		int wow;
		int weewoo;
	};
	
	uintptr_t paddr = pmm_alloc_block();
	vmm_map_page(paddr, MAIN_MEMORY_START);
	struct myStruct* myStruct = (struct myStruct*)MAIN_MEMORY_START;
	myStruct->wow = 42;
	myStruct->weewoo = 42;

	int i = 1;*/

	heap_init();

	run_tests();

    while(1);
    
    panic:
    return 1;
}
