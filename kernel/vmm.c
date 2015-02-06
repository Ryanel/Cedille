#include <stdint.h>
#include <stddef.h>
#include <logging.h>
void vmm_shim_doboardsetup();
void init_vmm() {
	vmm_shim_doboardsetup();
	printk(LOG_COMPLETE,"vmm","Started with no errors\n");
}
