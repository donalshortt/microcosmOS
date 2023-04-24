#include "test.h"

void test_ok(Suite suite)
{
	switch (suite)
	{
		case (PMM):
			print_string("OK pmm", 11, VGA_GREEN);
			break;
		case (VMM):
			print_string("OK vmm", 12, VGA_GREEN);
		default:
			return;
	}
}

void run_tests()
{
	if (pmm_run_tests() == 0) {
		test_ok(PMM);
	}

	if (vmm_run_tests() == 0) {
		test_ok(VMM);
	}
}

void fail(Test test)
{
	switch (test)
	{
		case PMM_ALLOC:
			print_string("FAIL pmm - alloc", 10, VGA_RED);
			break;
		case PMM_ALLOC_AND_FREE:
			print_string("FAIL pmm - alloc_and_free", 10, VGA_RED);
			break;
		case PMM_ALLOC_FREED:
			print_string("FAIL pmm - alloc_freed", 10, VGA_RED);
			break;
		case PMM_ALLOC_TWO_FREE_ONE:
			print_string("FAIL pmm - alloc_two_free_one", 10, VGA_RED);
			break;
		case PMM_ALLOC_1000_FREE_1000:
			print_string("FAIL pmm - alloc_1000_free_1000", 10, VGA_RED);
			break;
		case PMM_ALLOC_ALL_FREE_ALL:
			print_string("FAIL pmm - alloc_all_free_all", 10, VGA_RED);
			break;
		case VMM_MALLOC:
			print_string("FAIL vmm - malloc", 10, VGA_RED);
			break;
		case VMM_MALLOC_AND_FREE_AND_MALLOC:
			print_string("FAIL vmm- malloc_and_free_and_malloc", 10, VGA_RED);
			break;
	}
}

int assert_eq(long lhs, long rhs)
{
	if (lhs != rhs)
		return -1;

	return 0;
}
