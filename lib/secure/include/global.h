#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <config.h>

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
