#include <stddef.h>
#include <stdint.h>
#include <string.h>
int strcmp(const char* s1, const char* s2) {
	for (; (*s1 == *s2) && (*s1 != '\0'); s1++, s2++);
	return (*s1 == *s2) ? 0 : *s1 - *s2;
}
char * strcpy(char * dest, const char * src) {
	int len = strlen(src);
	memcpy(dest, src, len+1);
	return dest;
}
size_t strlen(const char *str)
{
	const char *wrk=str;
	int retval;
	for(retval = 0; *wrk != '\0'; wrk++) retval++;
	return retval;
}
char *strdup (char *s) {
	/**
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
    **/
    return NULL;
}
char * strtok_r(char * str, const char * delim, char ** saveptr)
{
	char * token;
	if (str == NULL) {
		str = *saveptr;
	}
	str += strspn(str, delim);
	if (*str == '\0') {
		*saveptr = str;
		return NULL;
	}
	token = str;
	str = strpbrk(token, delim);
	if (str == NULL) {
		*saveptr = (char *)lfind(token, '\0');
	} else {
		*str = '\0';
		*saveptr = str + 1;
	}
	return token;
}
size_t strspn(const char * str, const char * accept) {
	const char * ptr = str;
	const char * acc;

	while (*str) {
		for (acc = accept; *acc; ++acc) {
			if (*str == *acc) {
				break;
			}
		}
		if (*acc == '\0') {
			break;
		}

		str++;
	}

	return str - ptr;
}

char * strpbrk(const char * str, const char * accept) {
	const char *acc = accept;

	if (!*str) {
		return NULL;
	}

	while (*str) {
		for (acc = accept; *acc; ++acc) {
			if (*str == *acc) {
				break;
			}
		}
		if (*acc) {
			break;
		}
		++str;
	}

	if (*acc == '\0') {
		return NULL;
	}

	return (char *)str;
}
size_t lfind(const char * str, const char accept)
{
	size_t i = 0;
	while ( str[i] != accept) {
		i++;
	}
	return (size_t)(str) + i;
}