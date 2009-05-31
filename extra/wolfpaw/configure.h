#define INCL_STDLIB_H
#define INCL_UNISTD_H
#define INCL_TIME_H
#define USE_TZSET
#define INCL_SYS_TIMES_H
#define INCL_FCNTL_H
#define INCL_SYS_TIME_H
#define INCL_LIMITS_H
#define INCL_LOCALE_H
#define INCL_NETINET_IN_H
#define INCL_ARPA_INET_H
#define INCL_SYS_TYPES_H
#define INCL_SYS_IOCTL_H
#define INCL_SYS_SOCKET_H
#define INCL_NETDB_H
#define INCL_ARPA_TELNET_H
#define INCL_SYS_SOCKETVAR_H
#define INCL_SYS_STAT_H
#define INCL_DIRENT_H
#define USE_STRUCT_DIRENT
#define INCL_SYS_RESOURCE_H
#define INCL_SYS_WAIT_H
#define INCL_CRYPT_H
#define INCL_SYS_SYSMACROS_H
#define INCL_STDARG_H
#define INCL_DLFCN_H
#define DRAND48
#define USE_BSD_SIGNALS
#define SIGNAL_ERROR SIG_ERR
#define INLINE inline
#define CONST const
#define HAS_UALARM
#define HAS_STRERROR
#define HAS_GETCWD
#define RUSAGE
#define TIMES
#define HAS_GETTIMEOFDAY
#define HAS_FCHMOD
#define SIZEOF_INT 4
#define SIZEOF_PTR 4
#define SIZEOF_SHORT 2
#define SIZEOF_LONG 4
#define SIZEOF_FLOAT 4
#define UINT32 unsigned long
#define CONFIGURE_VERSION       5

#define MAXSHORT ((1 << (sizeof(short)*8)) - 1)
#define USHRT_MAX  (MAXSHORT)
#define USE_STRUCT_DIRENT
#define RTLD_LAZY     1
#define INADDR_NONE (unsigned int)0xffffffff
