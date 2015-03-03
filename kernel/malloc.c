#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <cedille/pmm.h>
#include <cedille/error.h>
#include <cedille/logging.h>
#define HEAP_BLOCK_MAGIC 0x0A5F1C54
#define CONFIG_PAGESIZE 0x1000 //TODO: MOVE TO KCONFIG.H
typedef struct heap_slab_slot
{
	struct heap_slab_slot * next;
	uint8_t data[];
} heap_slab_slot_t;

typedef struct heap_master_block
{
	struct heap_master_block * next;
	uint32_t magic; // Magic number to verify block validity.
	uint32_t bytes_left;
	uint32_t full;
	struct heap_block * list;
} heap_master_t;

typedef struct heap_block
{
	struct heap_block * next;
	struct heap_block * prev;
	uint32_t magic; // Magic number to verify block validity.
	uint32_t max;
	uint32_t used;
	uint32_t size;
	heap_slab_slot_t * slot_begin;
	heap_slab_slot_t * slot_end;
} heap_block_t;

int heap_allocatedBlocks = 0;

heap_master_t * heap_origin;

heap_block_t * heap_mmCreateBlock(uint32_t sz);

void heap_mmInit() {
	printf("Sizeof struct heap_block:%d bytes\n",sizeof(struct heap_block));
	heap_origin = (heap_master_t*) pmm_pfaAllocatePages(1);
	heap_origin->next = NULL;
	heap_origin->magic = HEAP_BLOCK_MAGIC;
	heap_origin->bytes_left = CONFIG_PAGESIZE - sizeof(heap_master_t);
	heap_origin->list = NULL;
}

heap_block_t * heap_mmCreateBlock(uint32_t sz) {
	heap_master_t * master_pos = heap_origin;
	while(true) {
		if(master_pos == NULL) {
			panic("No more free master blocks!");
			return NULL; // FIXME: Create a new master block.
		}
		if(master_pos->full) {

			master_pos = master_pos->next;
			continue;
		}
		if(master_pos->list == NULL) { //TODO: Review if its okay to skip a master block like this.
			printk(LOG_WARN,"slab","Ran into a NULL block at 0x%X! Ignoring.\n",&master_pos->list);
			master_pos = master_pos->next;
			continue;
		}

		heap_block_t * block_pos = master_pos->list;
		printk(LOG_WARN,"slab","Found block at 0x%X\n",block_pos);
		while(true) {
			if(block_pos->next == NULL) {
				uintptr_t * newblock = (uintptr_t*)block_pos;
				//TODO: Check if we can actually do this. If we can't, mark the master as filled.
				newblock = (uintptr_t *)newblock + sizeof(heap_block_t);
				printf("New block at 0x%X\n",newblock);
				block_pos->next = (heap_block_t*)newblock;
				block_pos->next->magic = HEAP_BLOCK_MAGIC;
				return (heap_block_t*)newblock;
			}
			block_pos = block_pos->next;
		}
	}
}