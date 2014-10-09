#include <arch/sparc/openprom.h>
#include <stddef.h>
#include <stdint.h>
prom_vec_t * prom_vec = NULL;

void kernel_entry(prom_vec_t * pv) {
    if(pv->pv_magic_cookie == ROMVEC_MAGIC)
    {
        asm("mov 0xFF, %g7");
        return;
    }
    prom_vec = pv;
    log(0,"Hello World!\n");
}