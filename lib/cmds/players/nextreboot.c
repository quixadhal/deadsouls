/*    /cmd *    from the Dead Souls Mudlib
 *    Shows when the next reboot occurs
 *    created by Descartes of Borg 940130
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string form) {
    string str, tz;
    int x, offset;

    if(DISABLE_REBOOTS){
        write("Recurring reboots are disabled.");
        write("There is no reboot scheduled by the events daemon.");
        return 1;
    }
    tz = this_player()->GetProperty("timezone");
    if(!tz || !valid_timezone(tz)) tz = query_tz();

    offset = TIME_D->GetOffset(tz);
    offset += EXTRA_TIME_OFFSET;

    x = EVENTS_D->GetRebootInterval() * 3600;
    x = (time() - uptime()) + x;
    if(query_os_type() != "windows" ) 
        x += offset * 3600;
    str = tz+ " " + ctime(x);
    message("system", "Current "+tz+" system time is "+timestamp(tz),
            this_player());
    if(form && form == "string") return "The next reboot will occur " + str + ".";
    else message("system", "The next reboot will occur " + str + ".",this_player());
    return 1;
}

string GetHelp(){
    return ("Syntax: nextreboot\n\n"
            "Tells you when the next regularly scheduled reboot for " +
            mud_name() + " will occur.");
}
