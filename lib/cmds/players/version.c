/*    /cmds/player/version.c
 *    from the Dead Souls LPC Library
 *    gives information about the MUD
 *    created by Descartes of Borg 950507
 */

#include <lib.h>
#include <daemons.h>
#include <localtime.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string tz, tmp;
    int x, scr, nr;

    tz = (string)this_player()->GetTimeZone() || localtime(time())[LT_ZONE];
    x = (int)TIME_D->GetOffset(tz) * 3600;
    scr = ((int *)this_player()->GetScreen())[0];
    nr = (time() - uptime()) + ((int)EVENTS_D->GetRebootInterval() * 3600);
    tmp = center(mud_name(), scr) + "\n";
    tmp += sprintf("%:-"+(scr/2)+"s%"+(scr/2)+"s\n", "Driver: " + version(),
		   "Library: " + mudlib() + " " + mudlib_version());
    tmp += sprintf("%:-" + (scr/2) + "s%" + (scr/2) + "s\n",
		  "Up since: " + ctime((time() - uptime()) +x),
		  "Next reboot: " + ctime(nr + x));
    tmp += center("Current time: " + ctime(time() + x) + " " + tz, scr);
    message("system", tmp, this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <version>\n\n"
      "Gives you version information about the LPMud as well as how "
      "long the mud has been up in your local time if you have it "
      "specified.", this_player()
    );
}
