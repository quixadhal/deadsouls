/*    /daemon/time.c
 *    from the Dead Souls LPC Library
 *    a time management daemon
 *    created by Descartes of Borg 950507
 */

#include <lib.h>
#include <cfg.h>
#include <localtime.h>
#include "include/time.h"

inherit LIB_DAEMON;

private static string LocalZone;
private static mapping Zones;

static void create() {
    string *lines;
    mixed *tmp;
    int i, x;

    daemon::create();
    Zones = ([]);
    tmp = localtime(time());
    LocalZone = tmp[LT_ZONE];
    x = tmp[LT_GMTOFF] / 3600;
    if( file_size(CFG_TIME) < 1 ) return;
    i = sizeof(lines = filter(explode(read_file(CFG_TIME), "\n"),
			      (: $1 && $1 != "" && $1[0] != '#' :)));
    while(i--) {
	string *words;

	if( sizeof(words = explode(lines[i], ":")) != 3 ) continue;
	Zones[words[0]] = new(class timezone);
	((class timezone)Zones[words[0]])->Offset = to_int(words[1]) - x;
	((class timezone)Zones[words[0]])->Name = words[2];
    }
}

int GetOffset(string tzone) {
    if( !Zones[tzone] ) return 0;
    else return ((class timezone)Zones[tzone])->Offset;
}

string GetName(string tzone) {
    if( !Zones[tzone] ) return 0;
    else return ((class timezone)Zones[tzone])->Name;
}

string *GetTimeZones() { return keys(Zones); }
