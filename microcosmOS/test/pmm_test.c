#include "../includes/test.h"
#include "../includes/pmm_test.h"
#include "../includes/front.h"
#include "../includes/pmm.h"
#include "../includes/types.h"
#include "../includes/constants.h"

int pmm_run_tests()
{
	if (alloc() == -1) { 
		fail(PMM_ALLOC);
		return -1;
	}

	if (alloc_and_free() == -1) {
		fail(PMM_ALLOC_AND_FREE);
		return -1;
	}

	if (alloc_freed() == -1) {
		fail(PMM_ALLOC_FREED);
		return -1;
	}

	if (alloc_two_free_one() == -1) {
		fail(PMM_ALLOC_TWO_FREE_ONE);
		return -1;
	}

	if (alloc_all_free_all() == -1) {
		fail(PMM_ALLOC_ALL_FREE_ALL);
		return -1;
	}

	return 0;
}

int alloc()
{
	void* expected_address = (void*) (get_first_free_block() * BLOCK_SIZE);
	void* block_address = pmm_alloc_block();

	return assert_eq((long)expected_address, (long)block_address);
}

int alloc_and_free()
{
	void* addr1 = (void*) (get_first_free_block() * BLOCK_SIZE);
	
	void* block_addr = pmm_alloc_block();
	pmm_dealloc_block(block_addr);
	
	void* addr2 = (void*) (get_first_free_block() * BLOCK_SIZE);

	return assert_eq((long)addr1, (long)addr2);
}

int alloc_freed()
{
	void* expected_address = (void*) (get_first_free_block() * BLOCK_SIZE);

	void* block_addr1 = pmm_alloc_block();
	pmm_dealloc_block(block_addr1);
	void* block_addr2 = pmm_alloc_block();

	return assert_eq((long) expected_address, (long) block_addr2);
}

int alloc_two_free_one()
{
	void* addr1 = pmm_alloc_block();
	void* ff_addr1 = (void*) (get_first_free_block() * BLOCK_SIZE);

	void* addr2 = pmm_alloc_block();
	pmm_dealloc_block(addr2);

	void* ff_addr2 = (void*) (get_first_free_block() * BLOCK_SIZE);

	return assert_eq((long) ff_addr1, (long) ff_addr2);
}

int alloc_all_free_all()
{
	int waow = pmm_state->max_blocks;

	return 0;
}

// alloc all memory, free all memory, first block should be free   
