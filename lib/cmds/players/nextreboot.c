/*    /cmd *    from the Dead Souls Mudlib 3.2
 *    Shows when the next reboot occurs
 *    created by Descartes of Borg 940130
 */

#include <lib.h>
#include <config.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string form) {
    string str;
    int x, offset;

    offset = (int)TIME_D->GetOffset(local_time()[9]);
    offset += EXTRA_TIME_OFFSET;

    x = (int)EVENTS_D->GetRebootInterval() * 3600;
    x = (time() - uptime()) + x;
    if(query_os_type() != "windows" ) 
	x += offset * 3600;
    str = query_tz()+ " " + ctime(x);
    message("system", "Current "+query_tz()+" system time is "+timestamp(),
      this_player());
    if(form && form == "string") return "The next reboot will occur " + str + ".";
    else message("system", "The next reboot will occur " + str + ".",this_player());
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <nextreboot>\n\n"
      "Tells you when the next regularly scheduled reboot for " +
      mud_name() + " will occur.");
}
