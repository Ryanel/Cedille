#include <assert.h>
#include <stddef.h>
#include <stdint.h>
uintptr_t * pmm_get_page() {
	return NULL;
}

void pmm_free_page(uintptr_t * page) {
	assert(page != NULL);
}