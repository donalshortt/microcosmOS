#include "vmm.h"

uint64_t pe_set_flag(pe pe, uint64_t flag) { return pe |= flag; };
uint64_t pe_del_flag(pe pe, uint64_t flag) { return pe &= ~flag; };
uint64_t pe_set_addr(pe pe, uint64_t addr) { return pe |= addr; };

//TODO: Standardise return codes!
/*int vmm_alloc_page(pte* pte)
{
    uintptr_t page_addr = pmm_alloc_block();

    if(!page_addr) {
        //TODO: Error
        return 1;
    }

    pe_set_flag(pte, PAGE_PRESENT);
    pe_set_flag(pte, PAGE_WRITEABLE);
    pe_set_addr(pte, page_addr);

    return 0;
}

void vmm_dealloc_page(pe* pe)
{
    void* addr = (void*)(*pe & PAGE_ADDR);

    if (!addr) {
        //TODO: Error
        return;
    }

    pmm_dealloc_block(addr);
    pe_del_flag(pe, PAGE_PRESENT);
}*/

__attribute__((unused)) void vmm_set_CR3(__attribute__((unused)) uint64_t pml4)
{
    __asm__ ( "movq [pml4], %rax" );
    __asm__ ( "movq %rax, %cr3" );
}

//TODO: Remove this shit
struct PML4* CURRENT_PML4;

__attribute__((unused)) int vmm_switch_pml4(struct PML4* pml4)
{
    if(!pml4) {
        //TODO: Error
        return 1;
    }

    CURRENT_PML4 = pml4;
    vmm_set_CR3((uint64_t)pml4);

	return 0;
}


uint64_t get_cr3_content()
{
    uint64_t rv;
    __asm__ __volatile__(
        "movq %%cr3, %0"
        : "=r"(rv)
    );
    return rv;
}

void flush_tlb()
{
	__asm__ __volatile__("wbinvd" : : : "memory");
}

#define extract_phys_addr(addr) (addr & 0xFFFFFF000)

// POSSIBLE BUG: what if the address that i want to map gets mapped to a page table hierarchy structure before the mapping process can complete?!
// POSSIBLE BUG: what if while in the process of allocing some memory, a page table structure is inserted in the middle of it? phys mem needs to be contiguous

//TODO: switch pmm_alloc_block(page) with kmalloc(page)

void vmm_map_page(uintptr_t virt)
{   
	//TODO: make sure the virt addr. is page-aligned
	if (!virt % 0x1000 != 0)
		return;

    struct PML4* pml4 = (struct PML4*) get_cr3_content();

	//1111111111111111111111111111000000000000

    uint64_t pml4_i  = PML4_GET_INDEX(virt);
    uint64_t pdpt_i  = PDPT_GET_INDEX(virt);
    uint64_t pd_i    = PD_GET_INDEX(virt);
    uint64_t pt_i    = PT_GET_INDEX(virt);

    struct PDPT* pdpt = 0;
    struct PD* pd = 0;
    struct PT* pt = 0;

    if ((pml4->entries[pml4_i] & PAGE_PRESENT) != PAGE_PRESENT) {
		pdpt = (struct PDPT*) pmm_alloc_block(page);
        
		pml4e pml4e = 0;

		pml4e = (uint64_t)pdpt;
		pml4e = pe_set_flag(pml4e, PAGE_PRESENT);
		pml4e = pe_set_flag(pml4e, PAGE_WRITEABLE);

		pml4->entries[pml4_i] = pml4e;
		vmm_map_page((uint64_t)pdpt);
		kmemset(pdpt, 0, PDPT_SIZE);
    } else {
		pdpt = (struct PDPT*) extract_phys_addr(pml4->entries[pml4_i]);
    }

    if ((pdpt->entries[pdpt_i] & PAGE_PRESENT) != PAGE_PRESENT) {
		pd = (struct PD*) pmm_alloc_block(page);
        
		pdpte pdpte = 0;

		pdpte = (uint64_t)pd;
		pdpte = pe_set_flag(pdpte, PAGE_PRESENT);
		pdpte = pe_set_flag(pdpte, PAGE_WRITEABLE);

		pdpt->entries[pdpt_i] = pdpte;
		vmm_map_page((uint64_t)pd);
		kmemset(pd, 0, PD_SIZE);
    } else {
		pd = (struct PD*) extract_phys_addr(pdpt->entries[pdpt_i]);
    }
    
    if ((pd->entries[pd_i] & PAGE_PRESENT) != PAGE_PRESENT) {
		pt = (struct PT*) pmm_alloc_block(page);
        
		pde pde = 0;

		pde = (uint64_t)pt;
		pde = pe_set_flag(pde, PAGE_PRESENT);
		pde = pe_set_flag(pde, PAGE_WRITEABLE);

		pd->entries[pd_i] = pde;
		vmm_map_page((uint64_t)pt);
		kmemset(pt, 0, PT_SIZE);
    } else {
		pt = (struct PT*) extract_phys_addr(pd->entries[pd_i]);
    }

	pte pte = 0;

	pte = (uint64_t)pmm_alloc_block(heap);
	pte = pe_set_flag(pte, PAGE_PRESENT);
	pte = pe_set_flag(pte, PAGE_WRITEABLE);

	pt->entries[pt_i] = pte;

	// TODO: don't flush the entire TLB
	flush_tlb();

	int wow = 42;
}

