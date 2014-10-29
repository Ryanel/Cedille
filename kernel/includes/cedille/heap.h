#ifndef HEAP_H
#define HEAP_H
#include <stdint.h>
#include <stddef.h>
void init_early_malloc(uintptr_t * address);
uintptr_t * early_malloc(uint32_t sz);
#endif