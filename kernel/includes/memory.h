#ifndef CEDILLE_MEM_H
#define CEDILLE_MEM_H
#include <stddef.h>
#include <stdint.h>
void init_pmm(uint32_t total_kb);
void init_malloc(uint32_t status);
uintptr_t * kmalloc(size_t sz);
uintptr_t * kmalloc_aligned(size_t sz);
uintptr_t * kmalloc_aligned_phys(size_t sz, uintptr_t * phys);
#endif