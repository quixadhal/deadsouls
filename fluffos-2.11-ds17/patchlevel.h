#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.11-ds17c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.11-ds17"
#else 
#define PATCH_LEVEL "v2.11-ds17w"
#endif
#endif
