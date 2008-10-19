#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.14-ds06c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.14-ds06"
#else 
#define PATCH_LEVEL "v2.14-ds06w"
#endif
#endif
