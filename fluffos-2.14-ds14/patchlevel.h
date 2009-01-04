#ifdef __CYGWIN__
#define PATCH_LEVEL "v2.14-ds14c"
#else
#ifndef WIN32
#define PATCH_LEVEL "v2.14-ds14"
#else 
#define PATCH_LEVEL "v2.14-ds14w"
#endif
#endif
