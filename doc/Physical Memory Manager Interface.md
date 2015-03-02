PMM
====================
The PMM is designed to be efficient and consise. It's split into two parts, the heap and the Page Frame Allocator (PFA). They both are referred to as the PMM.

Page Frame Allocator
--------------------
The page frame allocator interface itself does not care about the implementation. It only requires 4kb pages. It implements:

* `uintptr_t pmm_getTotalPages()` Returns the total amount of pages.
* `uintptr_t pmm_allocateSinglePage(uintptr_t address)` Allocates a page given the address.
* `uintptr_t pmm_allocatePages(uintptr_t n)` Allocates _n_ pages contiguously, and returns the address.
* `void pmm_freeSinglePage(uintptr_t address)` Frees a page given the address.
* `void pmm_freePages(uintptr_t address,uintptr_t n)` Frees _n_ contiguous pages.
* `void pmm_setMaxMemory(uintptr_t address)` Sets a pointer to the end of memory.
* `void pmm_init(uintptr_t address)` Initialses all structures.

Heap
--------------------
It's just liballoc.