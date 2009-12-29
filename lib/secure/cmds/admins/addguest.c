//      /bin/adm/_addguest.c
//      from the Dead Souls Mudlib
//      adds a person to the guest list for when the mud is locked
//      created by Descartes of Borg 08 july 1993

#include <lib.h>
#include <privs.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string *guests;
    mixed res;
    int i;

    if(!master()->valid_apply(({ PRIV_SECURE, PRIV_ASSIST }))){
        error("Illegal attempt to add a guest.");
    }

    if(!str) {
        notify_fail("Correct syntax: addguest [guestlist]\n");
        return 0;
    }
    i = sizeof(guests = explode(str, " "));
    while(i--) {
        if(!guests[i] || guests[i] == "") continue;
        if(res = catch(call_other(BANISH_D, "add_guest", guests[i]))) 
            message("admin", sprintf("Error in adding guest %s: %s",
                        guests[i], res), this_player());
    }
    message("admin", "Guests added.", this_player());
    return 1;
}

string GetHelp() {
    return ("Syntax: addguest [player list]\n\n"
            "Adds a single player or group of players to the database of guests "
            "allowed onto the mud when it is locked.  This is generally used to "
            "allow people in who are not in any of the groups defined in "
            "LOCKED_ACCESS_ALLOWED or to allow on player testers.\n"
            "See also: removeguests, whoguests");
}
