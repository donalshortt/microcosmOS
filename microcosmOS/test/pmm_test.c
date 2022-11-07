#include "../includes/test.h"
#include "../includes/pmm_test.h"
#include "../includes/front.h"
#include "../includes/pmm.h"
#include "../includes/types.h"
#include "../includes/constants.h"

int pmm_run_tests()
{
	if (alloc_block() == -1) { fail("alloc_block"); }
}

int alloc_block()
{
	void* expected_address = (void*) (get_first_free_block() * BLOCK_SIZE);

	void* block_address = pmm_alloc_block();

	return assert_eq((long)expected_address, (long)block_address);
}

int alloc_and_free_block()
{

}

int alloc_freed_block()
{

}
