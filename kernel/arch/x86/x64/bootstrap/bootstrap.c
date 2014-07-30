#include <stdint.h>
#include <multiboot.h>
void bs_init_console();
void print(const char *c);

int magic_no; 
multiboot_info_t * mb;
void bootstrap_init(int magic, multiboot_info_t * multiboot)
{
	//Save Data
	magic_no = magic;
	mb = multiboot;
	//Setup Console Immedately so we can print status
	bs_init_console();
	print("Cedille x64 Bootstrap - v 0.0.0.1 (c) Corwin McKnight 2014\n");
	if(magic_no != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		print("=> Boot was unsucessfull. Halting...");
		return;
	}
	print("=> Boot verified\n");
	int mods_loaded = multiboot->mods_count;
	if(mods_loaded == 0)
	{
		print("=> No kernel image was loaded into memory; Adjust your bootloader's paramaters.\n");
		print("=> Halting\n");
	}
	//Assume first passed module is kernel and try to boot it
	//Get its address
	print("=> Booting image in memory\n");
	int kernel_start = multiboot->mod_start;
	
}