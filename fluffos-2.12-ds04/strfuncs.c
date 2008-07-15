/*
 * ANSI C string library support/emulation
 *  (not complete)
 */

#if !defined(WIN32) && !defined(__CYGWIN__)

#include "std.h"

INLINE char *memset (char * s, int c, int n)
{
    if (c == 0)
	bzero(s, n);
    else {
	debug_message("Failed in memset\n");
	exit(1);
    }
}

INLINE char *memcpy (char * b, char * a, int s)
{
    bcopy(a, b, s);
    return b;
}

#endif
