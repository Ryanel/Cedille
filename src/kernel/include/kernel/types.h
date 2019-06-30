#pragma once

#include <stdint.h>

typedef uintptr_t virtual_address_t;
typedef uintptr_t phys_address_t;

// Define these in each platforms memory.cpp

virtual_address_t kPhysToVirtual(phys_address_t x);
phys_address_t kVirtualToPhys(virtual_address_t x);