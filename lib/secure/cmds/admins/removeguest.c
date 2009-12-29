//      /bin/adm/_removeguest.c
//      from the Dead Souls Mudlib
//      removes a person from the guest list for when the mud is locked
//      created by Descartes of Borg 08 july 1993

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string *guests;
    int i;

    if(!archp(previous_object())) return 0;
    if(!str) {
        notify_fail("Correct syntax: removeguest [guestlist]\n");
        return 0;
    }
    i = sizeof(guests = explode(str, " "));
    while(i--) BANISH_D->remove_guest(guests[i]);
    message("info", "Guests removed.", this_player());
    return 1;
}

string GetHelp(){
    return ("Syntax: removeguest [player list]\n\n"
            "Removes a single player or a list of players from the guests "
            "allowed onto the mud when it is locked.  This function is "
            "generally used to "
            "remove access a player might have been granted by the addguest "
            "command.\n"
            "See also: addguest, whoguests");
}
