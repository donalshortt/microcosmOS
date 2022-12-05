#ifndef _TEST_H_
#define _TEST_H_

#include "../front/front.h"
#include "pmm/pmm_test.h"
#include "vmm/vmm_test.h"

typedef enum Suite {
	PMM = 1, 
	VMM = 2
} Suite;

typedef enum Test {
	PMM_ALLOC = 1,
	PMM_ALLOC_AND_FREE = 2,
	PMM_ALLOC_FREED = 3,
	PMM_ALLOC_TWO_FREE_ONE = 4,
	PMM_ALLOC_1000_FREE_1000 = 5,
	PMM_ALLOC_ALL_FREE_ALL = 6
} Test;

void test_ok(Suite suite);

void run_tests();

void fail(Test test);

int assert_eq(long lhs, long rhs);

#endif
