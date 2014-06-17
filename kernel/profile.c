#include <stdio.h>
#include <memory.h>
#ifdef DEBUG
#ifndef DISABLE_PROFILER
#define START_PROFILE
#else
#define START_PROFILE return;
#endif
#endif
extern uint32_t memory_bytes_avalable;

void profiler_memory()
{
	START_PROFILE
	printf("Kernel Profiler: Memory\n");
	printf("Physical Address Space = 0x0 -> 0x%X\n",memory_bytes_avalable);
	uint32_t block_size = memory_bytes_avalable / (0x1000);
	if(block_size < 0x2000)
	{
		block_size = 0x2000;
	}
	printf("Used Memory (block size is 0x%X):\n",block_size);
		for (uint32_t i = 0; i < memory_bytes_avalable; i += block_size) {
		if(pmm_bitmap_test(i))
		{
			printf("%c",219);
		}
		else
		{
			printf("%c",176);
		}
	}
	uint32_t used = 0;
	uint32_t free = 0;
	uint32_t total = 0;
	for (uint32_t i = 0; i < memory_bytes_avalable; i += block_size) {
		if(pmm_bitmap_test(i))
		{
			used += 1;
		}
		else
		{
			free += 1;
		}
		total += 1;
	}
	printf("\n%d/%d blocks used\n",used,total);
}