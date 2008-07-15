#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.12-ds04c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.12-ds04"
#else 
#define PATCH_LEVEL "v2.12-ds04w"
#endif
#endif
