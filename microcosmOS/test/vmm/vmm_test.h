#ifndef __VMM_TEST_H__
#define __VMM_TEST_H__

#include "../test.h"
#include "../../front/front.h"
#include "../../mm/vmm/vmm.h"
#include "../../util/types.h"
#include "../../util/constants.h"

int vmm_run_tests();

int test_kmalloc();
int test_kmalloc_and_free_and_kmalloc();

#endif
