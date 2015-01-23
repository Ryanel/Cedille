int paging_setup();
int vmm_shim_doboardsetup() {
	int ret = paging_setup();
	return ret;
}