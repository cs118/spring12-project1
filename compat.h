#ifndef COMPAT_H
#define COMPAT_H

#include <stddef.h>

#ifndef HAVE_MEMMEM
/* Return the first occurrence of NEEDLE in HAYSTACK.  */
void *
memmem (const void *haystack, size_t haystack_len, const void *needle,
	size_t needle_len);
#endif

#ifndef HAVE_STPNCPY
char *
stpncpy(char *s1, const char *s2, size_t n);
#endif

#endif
