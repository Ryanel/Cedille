int paging_setup();
int vmm_shim_doBoardSetup() {
	int ret = paging_setup();
	return ret;
}