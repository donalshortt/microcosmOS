#ifndef __PMM_TEST_H__
#define __PMM_TEST_H__

#include "../../test/test.h"
#include "../../front/front.h"
#include "../../mm/pmm/pmm.h"
#include "../../util/types.h"
#include "../../util//constants.h"

int pmm_run_tests();

int test_alloc();
int test_alloc_and_free();
int test_alloc_freed();
int test_alloc_two_free_one();
int test_alloc_1000_free_1000();
int test_alloc_all_free_all();

#endif
