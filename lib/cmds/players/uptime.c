/*    /cmds/player/uptime.c
 *    from the Dead Souls LPC Library
 *    tells how long the driver has been up
 *    created by Beek@Dead Souls 950309
 */

#include <lib.h>

#define MIN 60
#define HOUR (60 * MIN)
#define DAY  (24 * HOUR)
#define WEEK ( 7 * DAY)

inherit LIB_DAEMON;

mixed cmd(string unused) {
    int tm = uptime();
    string str;
    int x;

    str = mud_name() + " has been up for %^ORANGE%^";
    if (x = (tm / WEEK)) {
        str += x + "w ";
        tm -= x * WEEK;
    }
    if (x = (tm / DAY)) {
        str += x +"d ";
        tm -= x * DAY;
    }
    if (x = (tm / HOUR)) {
        str += x + "h ";
        tm -= x * HOUR;
    }
    if (x = (tm / MIN)) {
        str += x + "m ";
        tm -= x * MIN;
    }
    if (tm) {
        str += tm + "s ";
    }
    str = str[0..<2] + "%^RESET%^.";
    write(str);
    return 1;
}

string GetHelp(){
    return ("Syntax: uptime\n\nReports how long "+mud_name()+" has been up.");
}
