#include "../util/gdt.h"
#include "../util/mmu.h"
#include "../util/multiboot2.h"
#include "../util/constants.h"

.SET HEADER_LENGTH, header_end - header_start
.SET CHECKSUM, -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + HEADER_LENGTH)
.section .multiboot
header_start:
    .long MULTIBOOT2_HEADER_MAGIC
    .long MULTIBOOT_ARCHITECTURE_I386
    .long HEADER_LENGTH
    .long CHECKSUM

    // multiboot tags go here

    .short MULTIBOOT_HEADER_TAG_END
    .short 0    // flags, none set
	.long 8     // size, including itself (short + short + long)
header_end:

.code32

.section .bss
.comm pml4, PML4_SIZE, PML4_ALIGNMENT
.comm low_pdpt, PDPT_SIZE, PDPT_ALIGNMENT
.comm high_pdpt, PDPT_SIZE, PDPT_ALIGNMENT
.comm low_page_directory_table, PAGE_DIRECTORY_SIZE, PAGE_DIRECTORY_ALIGNMENT
.comm high_page_directory_table, PAGE_DIRECTORY_SIZE, PAGE_DIRECTORY_ALIGNMENT
.comm tmp_stack, BOOT_STACK_SIZE, BOOT_STACK_ALIGN

.data
.align GDT_TABLE_ALIGNMENT
gdt_table:
	.8byte GDT_FIRST_ENTRY
	.8byte GDT_KERNEL_ENTRY

gdt_table_end:
	.skip (GDT_TABLE_SIZE - (gdt_table_end - gdt_table))

gdt_ptr:
	.short GDT_TABLE_SIZE - 1
	.long gdt_table
	

.section .text
.global _start
.type _start, @function
_start:
	movl $tmp_stack + BOOT_STACK_SIZE, %esp

	/* Set two entries in the PML4 table, each pointing to a pdpt */
	/* TODO: why does it set an entry up for both physical and virtual?*/
	movl $low_pdpt, %eax
	or $(MMU_PRESENT | MMU_WRITABLE), %eax
	movl %eax, pml4 + (PML4_ADDR_TO_ENTRY_INDEX(OS_PHYS_START) * PML4_ENTRY_SIZE)

	movl $high_pdpt, %eax
	or $(MMU_PRESENT | MMU_WRITABLE), %eax
	movl %eax, pml4 + (PML4_ADDR_TO_ENTRY_INDEX(OS_VIRT_START) * PML4_ENTRY_SIZE)

	/* Set an entry for each PDPT... for the physical and virtual*/
	movl $low_page_directory_table, %eax
	or $(MMU_PRESENT | MMU_WRITABLE), %eax
	movl %eax, low_pdpt + (PDPT_ADDR_TO_ENTRY_INDEX(OS_PHYS_START) * PDPT_ENTRY_SIZE)

	movl $high_page_directory_table, %eax
	or $(MMU_PRESENT | MMU_WRITABLE), %eax
	movl %eax, high_pdpt + (PDPT_ADDR_TO_ENTRY_INDEX(OS_VIRT_START) * PDPT_ENTRY_SIZE)

	mov $0, %ecx

	/* 2mb is the amount of space that is taken up by 512 tables of size 4096 bytes each*/
	/* esi should contain the number of pages we want*/
	/* so this is just calculating the number of pages that we need based on the physical size of the kernel*/
	movl $_kernel_physical_end, %esi
	shrl $TWO_MEGABYTES_SHIFT, %esi // this divides
	addl $1, %esi

page_directory_table_loop:
	movl $TWO_MEGABYTES, %eax
	mul %ecx
	/* No. of pages calculated ^^^*/
	or $(MMU_PRESENT | MMU_WRITABLE | MMU_PDE_TWO_MB), %eax
	movl %eax, low_page_directory_table(, %ecx, PAGE_DIRECTORY_ENTRY_SIZE)
	movl %eax, high_page_directory_table(, %ecx, PAGE_DIRECTORY_ENTRY_SIZE)
	/* Store the two entries in the low and high page tables respectively (low = phys, high = virt) */

	inc %ecx
	cmp %esi, %ecx
	jne page_directory_table_loop

	/* Point cr3 to pml4 */
	movl $pml4, %eax
	movl %eax, %cr3

	/* set cr4 flags; (enable PAE) */
	movl $CR4_FINAL, %eax
	movl %eax, %cr4

	/* enable long mode */
	movl $MSR_EFER_ADDR, %ecx
	rdmsr
	or $MSR_EFER_FINAL, %eax
	wrmsr

	/* set cr0 flags; (protected mode, extension type, enable paging) */
	movl $CR0_FINAL, %eax
	movl %eax, %cr0

	lgdt gdt_ptr

	ljmp $(GDT_ENTRY_SIZE), $_start64

	cli
	hlt

.code64

.global _start64
.type _start64, @function
_start64:
        movw $0, %ax
        /* TODO: Check if necessary */
        movw %ax, %ds
        movw %ax, %es
        movw %ax, %fs
        movw %ax, %gs
        movw %ax, %ss

        movl %ebx, %edi

        call kernel_main

        /* Should never reach here*/
        cli
        hlt
1:
        jmp 1b
