#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string which, nom;

    if(!archp(previous_object())) return 0;
    if(!str) {
        notify_fail("Watch which name or site?\n");
        return 1;
    }
    if(sscanf(str, "%s %s", which, nom) != 2) {
        notify_fail("Correct syntax: <watch [name|site] [ip or name]>\n");
        return 1;
    }
    if(which == "name") BANISH_D->watch_name(nom);
    else if(which == "site") BANISH_D->watch_site(nom);
    else {
        notify_fail("Undefined watch type: "+str+"\n");
        return 1;
    }
    message("info", nom+" is now an watched "+which, this_player());
    return 1;
}

string GetHelp(){
    return ("Syntax: watch <[site|name] | [ip or site to be watched]>\n\n"
            "Sets it up so that whenever a player of a certain name, or "
            "a player from a certain site logs in it will get logged.  "
            "Examples: watch name forlock    watch site 134.181.*\n"
            "See also: unwatch, whowatched");
}
