#include <string.h>

char *strncpy(char *destination, const char *source, size_t n)
{
    size_t size = strlen(source);
    if (size > n)
    {
        size = n;
    }
    if (size != n)
    {
        memset(destination + size, '\0', n - size);
    }

    return memcpy(destination, source, size);
}