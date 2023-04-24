#include "vmm_test.h"

int vmm_run_tests()
{
	if (test_kmalloc() == -1) {
		fail(VMM_MALLOC);
		return -1;
	}

	if (test_kmalloc_and_free_and_kmalloc() == -1) {
		fail(VMM_MALLOC_AND_FREE_AND_MALLOC);
		return -1;
	}

	return 0;
}

int test_kmalloc()
{
	int* ptr = kmalloc(sizeof(int));
	
	return assert_eq((long)ptr, MAIN_MEMORY_START + BLOCK_SIZE);
}

int test_kmalloc_and_free_and_kmalloc()
{
	int* ptr = kmalloc(sizeof(int));

	kfree(ptr);

	int* ptr2 = kmalloc(sizeof(int));

	return assert_eq((long)ptr, (long)ptr2);
}
