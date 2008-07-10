#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.11-ds18c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.11-ds18"
#else 
#define PATCH_LEVEL "v2.11-ds18w"
#endif
#endif
