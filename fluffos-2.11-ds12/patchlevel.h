#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.11-ds12c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.11-ds12"
#else 
#define PATCH_LEVEL "v2.11-ds12w"
#endif
#endif
