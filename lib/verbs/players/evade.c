/*  Allows players to possibly evade a targets
 *  created by Rush@Dead Souls
 */

#include <lib.h>
#include "include/evade.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("evade");
    SetRules("LIV");
    SetErrorMessage("Who are you trying to evade?");
    SetHelp("Syntax: evade <LIVING>\n\n"
            "Allows one to attempt to evade a living being "
            "who is currently following.\n"
            "See also: follow, lead, tracking, stealth\n");
}

mixed can_evade_liv() { return 1; }

mixed do_evade_liv(object ob) {
    if(!this_player()->SetAllowed(ob, 0)){
        this_player()->eventPrint("You are now evading " + 
                ob->GetName() + ".");
    }
    else this_player()->eventPrint("You are not empowered to evade " + ob->GetName() + ".");
    return 1;
}
