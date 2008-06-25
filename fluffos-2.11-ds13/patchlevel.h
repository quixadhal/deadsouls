#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.11-ds13c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.11-ds13"
#else 
#define PATCH_LEVEL "v2.11-ds13w"
#endif
#endif
