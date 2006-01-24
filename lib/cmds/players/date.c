/*  /cmds/player/date.c
 *  A simple command which displays current date and time
 *  Blitz@NM-IVD
*/

#include <lib.h>
#include <config.h>
#include <daemons.h>
#include <localtime.h>

mixed cmd(string unused) {
    string *parts, year, time;
    int offset, x, hour, min, sec;

    offset = (int)TIME_D->GetOffset(local_time()[9]);
    offset += EXTRA_TIME_OFFSET;
    if(query_os_type() != "windows" ) 
	x = offset * 3600;
    else x = 0;
    time = ctime( time() + x );
    x = sizeof(parts = explode(time, " "));
    year = parts[x - 1];
    sscanf(parts[x - 2], "%d:%d:%d", hour, min, sec);
    if( !hour ) hour = 12;
    message("info",
      sprintf("Time: %d:%s%d %s\nDate: %s, %s",
	(hour>12 ? (hour-12) : hour),
	(min < 10 ? "0" : ""),
	min,
	(hour>11 ? "pm" : "am"),
	implode(parts[0..(x-3)], " "),
	year),
      this_player() );
    return 1;
}

void help() {
    message("help",
      "Syntax: date\n\n"
      "Displays current time and date in local time.",
      this_player() );
}
