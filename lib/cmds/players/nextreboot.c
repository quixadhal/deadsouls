/*    /cmd *    from the Dead Souls Mudlib 3.2
 *    Shows when the next reboot occurs
 *    created by Descartes of Borg 940130
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string tzone;
    int x;

    x = (int)EVENTS_D->GetRebootInterval() * 3600;
    x = (time() - uptime()) + x;
    if( tzone = (string)this_player()->GetTimeZone() )
      x += (int)TIME_D->GetOffset(tzone) * 3600;
    else tzone = "CST";
    str = tzone + " " + ctime(x);
    message("system", "The next reboot will occur " + str + ".",this_player());
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <nextreboot>\n\n"
	    "Tells you when the next regularly scheduled reboot for " +
	    mud_name() + " will occur.");
}
