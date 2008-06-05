#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.11-ds07c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.11-ds07"
#else 
#define PATCH_LEVEL "v2.11-ds07w"
#endif
#endif
