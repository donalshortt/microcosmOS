#include "../includes/front.h"
#include "../includes/pmm_test.h"

#ifndef _TEST_H_
#define _TEST_H_

enum Suite {
	PMM = 1, 
	VMM = 2
} typedef Suite;

enum Test {
	PMM_ALLOC_BLOCK = 1,
	PMM_ALLOC_AND_FREE_BLOCK = 2,
	PMM_ALLOC_FREED_BLOCK = 3
} typedef Test;

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
		case PMM_ALLOC_BLOCK:
			print_string("FAIL pmm - alloc_block", 10, VGA_RED);
			break;
		case PMM_ALLOC_AND_FREE_BLOCK:
			print_string("FAIL pmm - alloc_and_free_block", 10, VGA_RED);
			break;
		case PMM_ALLOC_FREED_BLOCK:
			print_string("FAIL pmm - alloc_freed_block", 10, VGA_RED);
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

#endif
