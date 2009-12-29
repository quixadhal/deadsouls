// _tempban.c
// /cmd// Part of the Dead Souls Mudlib
// A command so that all those law-types can place sites on registration for a 
// specified, limited amount of time.
// 01July1994 by Gregon@Dead Souls

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string site;
    int time;

    if(!archp(previous_object())) return 0;

    if(!str) {
        write("The following sites are on temporary registration:\n"+
                BANISH_D->query_temp_site_info());
        return 1;
    }


    if(sscanf(str, "%s %d", site, time) != 2) {
        notify_fail("Correct syntax: tempban [site] [time in minutes]>\n\n");
        return 0;
    }

    write(site+" is now on register for "+time+" minutes.");
    BANISH_D->temporary_register(site, time*60);
    log_file("watch/register", site+" for "+time+" minutes.\n");
    log_file("watch/register", "by " +this_player()->GetName()+"\n");
    return 1;
}

string GetHelp(){
    return ("Syntax: tempban [site] [time]\n\n"
            "This will make new players from "
            "[site] unable to log in for a period of time [time] "
            "(given in minutes).  This will last across reboots and site ip "
            "numbers should be entered in the same manner as the \"register\" "
            "command requires.  See \"help register\" for more details.\n\n"
            "Note: tempban without arguments will list all sites on temporary "
            "registration along with their times of expiration.");
}
