#include "../includes/test.h"

void test_ok(Suite suite)
{
	switch (suite)
	{
		case (PMM):
			print_string("OK pmm", 11, VGA_GREEN);
			break;
		default:
			return;
	}
}

void run_tests()
{
	if (pmm_run_tests() == 0) {
		test_ok(PMM);
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
		case PMM_ALLOC_ALL_FREE_ALL:
			print_string("FAIL pmm - alloc_all_free_all", 10, VGA_RED);
			break;
		defualt:
			return;
	}
}

int assert_eq(long lhs, long rhs)
{
	if (lhs != rhs)
		return -1;

	return 0;
}
