#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <string.h>
#include <logging.h>
#include <cedille.h>
#include <arch/x86/paging.h>
#include <arch/x86/descriptor_tables.h>
extern uint32_t _kernel_end;

page_directory_t * kernel_page_directory = NULL;
page_directory_t * current_page_directory = NULL;

uintptr_t mman_get_placement_address();

void vmm_identity_map(uint32_t start_addr, uint32_t end_addr)
{
	if(start_addr > end_addr)
	{
		oops("VMM indentity map OOB");
	}
	uint32_t addr = start_addr;
	addr &= 0xFFFFF000;
	while(addr < end_addr)
	{
		pmm_alloc_frame(pmm_get_page(kernel_page_directory,addr, 1), 1,1);
		addr += 0x1000;
	}
	asm("hlt");
}

void vmm_set_page_directory(page_directory_t *d)
{
	current_page_directory = d;
	asm volatile("mov %0, %%cr3":: "r"(&d->tablesPhysical));
}

void vmm_enable_paging()
{
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; //Sets the page enable bit in cr0
	asm volatile("mov %0, %%cr0":: "r"(cr0));
	printk("cpu","Paging enabled\n");
}

void vmm_page_fault_exception(struct regs *regs)
{
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	// The error code gives us details of what happened.
	int present   = !(regs->err_code & 0x1); // Page not present
	int rw = regs->err_code & 0x2;           // Write operation?
	int us = regs->err_code & 0x4;           // Processor was in user-mode?
	int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
	int id = regs->err_code & 0x10;          // Caused by an instruction fetch?
	printk("fault","Page fault at 0x%x ( ",faulting_address);
	if (present) {printf("present ");}
	if (rw) {printf("read-only ");}
	if (us) {printf("user-mode ");}
	if (reserved) {printf("reserved ");}
	printf(")\n");
}

void init_vmm()
{
	printk("info","Starting Virtual Memory Manager...\n");
	kernel_page_directory = (page_directory_t *)kmalloc_aligned(sizeof(page_directory_t));
	memset(kernel_page_directory, 0, sizeof(page_directory_t));
	current_page_directory = kernel_page_directory;
	vmm_identity_map(0x0,mman_get_placement_address());
	vmm_set_page_directory(kernel_page_directory);
	register_interrupt_handler (14, vmm_page_fault_exception); //Setup Page Fault Handler
	vmm_enable_paging();
}
