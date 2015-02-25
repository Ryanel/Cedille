#ifndef HEAP_H
#define HEAP_H
#include <stdint.h>
#include <stddef.h>
void init_early_malloc(uintptr_t * address);
uintptr_t * early_malloc_aligned(uint32_t sz);
uintptr_t * early_malloc_aligned_pz(uint32_t sz,uintptr_t *phys);
uintptr_t * early_malloc(uint32_t sz);

void heap_init();

#endif
