//      /adm/simul_efun/mud_info.c
//      from the Dead Souls Mudlib
//      gives information simul_efuns about the mud
//      created by Descartes of Borg 930814

#include <config.h>

string mud_name() { return MUD_NAME; }

#ifdef MUDOS_VERSION
string version() { return MUDOS_VERSION; }
#endif // MUDOS_VERSION

#ifdef __VERSION__
string version() { return __VERSION__; }
#endif // __VERSION__

#ifdef MUDOS_ARCH
string architecture() { return MUDOS_ARCH; }
#endif // MUDOS_ARCH

#ifdef __ARCH__
string architecture() { return __ARCH__; }
#endif // __ARCH__

/* Changing this value with prior consent is a violation of the Dead Souls
 * Object Library License Agreement and will result in the loss of
 * your right to use Dead Souls
 */

/* Interestingly, the warning above seems to forbid the attempt
 * to seek permission. Isn't that odd? Anyway, Dead Souls as released
 * was public domain, and is now GPL, so you do what you want. 
 * I'm leaving that there as a
 * reminder of how totalitarian and narrow-minded some people can
 * be. It would be nice if you left the mudlib name and revision
 * alone, though, so we can count you among our brother muds.
 * - Cratylus @ Frontiers
 */

string mudlib() { return "Dead Souls"; }

string mudlib_version() { return "1.1r4"; }

int query_host_port() { return __PORT__; }
