#ifndef _TEST_H_
#define _TEST_H_

#include "../includes/front.h"
#include "../includes/pmm_test.h"

typedef enum Suite {
	PMM = 1, 
	VMM = 2
} Suite;

typedef enum Test {
	PMM_ALLOC_BLOCK = 1,
	PMM_ALLOC_AND_FREE_BLOCK = 2,
	PMM_ALLOC_FREED_BLOCK = 3
} Test;

void test_ok(Suite suite);

void run_tests();

void fail(Test test);

int assert_eq(long lhs, long rhs);

#endif
