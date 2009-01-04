#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(mixed args) {
    int *mudtime;
    int integer, hours, minutes;
    string meridiem = " am";
    string nulle = "";

    if(!archp(previous_object())) return 0;
    if(!args) args = 0;
    if(stringp(args)){
        if(sscanf(args,"%d",integer) != 1) return "Failed.";
        args = integer;
    }

    if(args != 0 || SEASONS_D->GetTickTock()) SEASONS_D->eventTickTock(args);

    mudtime = SEASONS_D->GetMudTime();
    hours = mudtime[0];
    minutes = mudtime[1];

    //tod = SEASONS_D->GetTimeOfDay();

    if(hours >= 12  && hours != 24) {
        if(hours != 12) hours -= 12;
        meridiem = " pm";
    }

    if(!hours || hours == 0) hours = 12;

    if(minutes < 10) nulle = "0";

    write("In the MUD, the time is "+hours+":"+nulle+minutes+meridiem+"."); 

    return 1;

}

void help() {
    message("help", "Syntax: <ticktock [seconds]>\n\n"
            "Changes in-game mud time by the number of seconds "
            "specified. This is useful for changing night to "
            "day and vice versa, when testing ambient light and "
            "variable room descriptions. The timeshift does not "
            "persist between reboots.\n\n"
            "", this_player());
}
