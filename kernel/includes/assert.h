#ifndef ASSERT_H
#define ASSERT_H
#if __STDC_VERSION__ == 199901L
void k_assert_99( char const * const, char const * const, char const * const );
#else
void k_assert( char const * const );
#endif
#define __symbol2value( x ) #x
#define __symbol2string( x ) __symbol2value( x )
#endif

/* re-including assert.h results in assert() being redefined */
#undef assert

#ifdef NDEBUG
#define assert( ignore ) ( (void) 0 )
#else
#if __STDC_VERSION__ == 199901L
#define assert( expression ) ( ( expression ) ? (void) 0 \
        : k_assert_99( "Assertion failed: " #expression \
                          ", function ", __func__, \
                          "file: " __FILE__ \
                          ", line " __symbol2string( __LINE__ ) \
                          "." ) )
#else
#define assert( expression ) ( ( expression ) ? (void) 0 \
        : k_assert( "Assertion failed: " #expression \
                          ", file " __FILE__ \
                          ", line " __symbol2string( __LINE__ ) \
                          "." ) )
#endif
#endif