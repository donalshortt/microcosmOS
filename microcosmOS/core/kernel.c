#include "../util/multiboot2.h"
#include "../mm/mm.h"
#include "../mm/pmm/pmm.h"
#include "../mm/vmm/vmm.h"
#include "../front/front.h"
#include "../util/types.h"
#include "../test/test.h"
#include "../vulns/vulns.h"

int kernel_main(unsigned long boot_info_addr)
{
    
	//TODO: Remove this magic number
	if (boot_info_addr & 7) {
		print_string("Incorrect alignment for multiboot info", 0, VGA_RED);
		goto panic;
	}

	print_name();

	setup_pmm(boot_info_addr);

	run_tests();

	unsigned long l1_average = get_average_time_l1();
	unsigned long main_mem_average = get_average_time_main_mem();

	int wow = 1;
	while(1);
    
	panic:
	return 1;
}
