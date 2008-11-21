#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.14-ds13c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.14-ds13"
#else 
#define PATCH_LEVEL "v2.14-ds13w"
#endif
#endif
