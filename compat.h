#ifndef COMPAT_H
#define COMPAT_H

#include <stddef.h>

#ifndef HAVE_MEMMEM
/* Return the first occurrence of NEEDLE in HAYSTACK.  */
void *
memmem (const void *haystack, size_t haystack_len, const void *needle,
	size_t needle_len);
#endif

#ifndef HAVE_STRNCPY
char *
strncpy(char *s1, const char *s2, size_t n);
#endif

#endif
