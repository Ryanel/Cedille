#include <stdint.h>
#include <stddef.h>
#include <logging.h>
void vmm_shim_doboardsetup();
void init_vmm() {
	vmm_shim_doboardsetup();
	printk("ok","VMM started with no errors\n");
}
