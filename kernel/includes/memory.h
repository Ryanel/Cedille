#ifndef CEDILLE_MEM_H
#define CEDILLE_MEM_H
#include <stddef.h>
#include <stdint.h>
void init_pmm(uint32_t total_kb);
uintptr_t * kmalloc(size_t sz);
void init_malloc(uint32_t status);
#endif