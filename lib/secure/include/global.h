#ifndef __GLOBAL_H
#define __GLOBAL_H

#define CONFIG_H "/secure/include/config.h"
#define NETWORK_H "/secure/include/network.h"
#define ROOMS_H "/secure/include/rooms.h"
#define SECRETS_H "/secure/include/secrets.h"

#include CONFIG_H
#include <lib.h>

#if COMPAT_MODE
#include <compat.h>
#endif

#define DAY_ONE           720550800

#define SEFUN             "/secure/sefun/sefun"

#define DEBUG

#ifdef debug
#undef debug
#endif /* debug */

//#ifdef DEBUG
//#define debug(x, y)      (find_player(x) || master())->eventPrint(x)
//#endif /* DEBUG */

#endif /* GLOBAL_H */
