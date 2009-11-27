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
    string tz, tmp, extra;
    int offset, x, scr, nr;

    if(sizeof(query_os_type())) extra = " for "+query_os_type();
    tz = this_player()->GetProperty("timezone");
    if(!tz || !valid_timezone(tz)) tz = query_tz();
    offset = TIME_D->GetOffset(tz);
    offset += EXTRA_TIME_OFFSET;
    if(query_os_type() != "windows" ) x = offset * 3600;
    else x = 0;
    scr = ((int *)this_player()->GetScreen())[0];
    nr = (time() - uptime()) + (EVENTS_D->GetRebootInterval() * 3600);
    tmp = center(mud_name(), scr) + "\n";
    tmp += sprintf("%:-"+(scr/2)+"s%"+(scr/2)+"s\n", "Driver: " + version(),
            "Library: " + mudlib() + " " + mudlib_version()+extra);
    tmp += sprintf("%:-" + (scr/2) + "s%" + (scr/2) + "s\n",
            "Up since: " + ctime((time() - uptime()) +x),
            "Next reboot: " + ctime(nr + x));
    tmp += center("Current time: " + ctime(time() + x) + " " + tz, scr);
    message("system", tmp, this_player());
    return 1;
}

string GetHelp() {
    return ("Syntax: version\n\n"
            "Gives you version information about "+mud_name()+" as well as how "
            "long the mud has been up in your local time if you have it "
            "specified.");
}
