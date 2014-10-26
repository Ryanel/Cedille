#include <assert.h>
#include <logging.h>

void panic(char * s);

void k_assert( char const * const s) {
	printk("error","%s\n",s);
	panic("Assertion failed");
}

void k_assert_99( char const * const a , char const * const b, char const * const c)
{
	printk("error","%s%s\n\t%s\n",a,b,c);
	panic("Assertion failed");
}