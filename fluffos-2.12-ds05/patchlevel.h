#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.12-ds05c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.12-ds05"
#else 
#define PATCH_LEVEL "v2.12-ds05w"
#endif
#endif
