#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.14-ds02c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.14-ds02"
#else 
#define PATCH_LEVEL "v2.14-ds02w"
#endif
#endif
