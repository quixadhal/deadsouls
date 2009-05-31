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
    //LocalZone = tmp[LT_ZONE];
    if(LOCAL_TIME) x = -(tmp[LT_GMTOFF] / 3600);
    else x = 0;
    if( file_size(CFG_TIME) < 1 ) return;
    i = sizeof(lines = filter(explode(read_file(CFG_TIME), "\n"),
                (: $1 && $1 != "" && $1[0] != '#' :)));
    foreach(string line in lines) {
        string *words;
        if( sizeof(words = explode(line, ":")) != 3 ) continue;
        Zones[words[0]] = ([]);
        Zones[words[0]]["offset"] = to_int(words[1]) - x;
        Zones[words[0]]["name"] = words[2];
    }
}

int GetOffset(string tzone) {
    if(LOCAL_TIME) return 0;
    if(!tzone) tzone = query_tz();
    if( !Zones[tzone] ) return 0;
    else return Zones[tzone]["offset"];
}

string GetName(string tzone) {
    if(!tzone) tzone = query_tz();
    if( !Zones[tzone] ) return 0;
    else return Zones[tzone]["name"];
}

string *GetTimeZones() { return keys(Zones); }
