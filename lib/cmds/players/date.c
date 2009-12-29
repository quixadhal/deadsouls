/*  /cmds/player/date.c
 *  A simple command which displays current date and time
 *  Blitz@NM-IVD
 */

#include <lib.h>
#include <daemons.h>
#include <localtime.h>

mixed cmd(string timezone) {
    string *parts, year, time, tz;
    int offset, x, hour, min, sec;

    if(!timezone || !valid_timezone(timezone)){
        timezone = this_player()->GetProperty("timezone");
    }
    if(!timezone || !valid_timezone(timezone)) timezone = local_time()[9];

    offset = TIME_D->GetOffset(timezone);
    offset += EXTRA_TIME_OFFSET;
    if(query_os_type() != "windows" ) 
        x = offset * 3600;
    else x = 0;
    time = ctime( time() + x );
    x = sizeof(parts = explode(time, " "));
    year = parts[x - 1];
    sscanf(parts[x - 2], "%d:%d:%d", hour, min, sec);
    message("info",
            sprintf("Time: %d:%s%d %s\nDate: %s, %s",
                (hour>12 ? (hour-12) : (hour == 0 ? 12 : hour)),
                (min < 10 ? "0" : ""),
                min,
                ((hour>11 && hour) ? "pm" : "am"),
                implode(parts[0..(x-3)], " "),
                year),
            this_player() );
    return 1;
}

string GetHelp() {
    return ("Syntax: date\n\n"
            "Displays current time and date in local time.");
}
