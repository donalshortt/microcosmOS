#include "../util/multiboot2.h"
#include "../mm/mm.h"
#include "../mm/pmm/pmm.h"
#include "../mm/vmm/vmm.h"
#include "../front/front.h"
#include "../util/types.h"
#include "../test/test.h"
#include "../vulns/vulns.h"
#include "../vulns/flush_reload/flush_reload.h"
#include "../vulns/spectre/spectre.h"

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

	flush_reload();
	spectre();

	while(1);
    
	panic:
	return 1;
}
