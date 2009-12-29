/*  Allows players to possibly help a target follow.
 *  created by Rush@Dead Souls
 */

#include <lib.h>
#include "include/lead.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("lead");
    SetRules("LIV");
    SetErrorMessage("Whom would you like to lead?");
    SetHelp("Syntax: lead <LIVING>\n\n"    
            "Allows one to assist a living being who is "
            "attempting to follow.\n"
            "See also: evade, follow, tracking, stealth\n");
}

mixed can_lead_liv() { return 1; }

mixed do_lead_liv(object ob) {
    if(!ob->IsFollowing(this_player())) {
        this_player()->eventPrint(ob->GetName() + " is not following you.");
        return 1;
    }
    if( this_player()->SetFollowed(ob, 1) ) {
        ob->eventPrint(this_player()->GetName() + " is now leading you.");
        this_player()->eventPrint("You are now leading " + ob->GetName() + ".");
    }
    else this_player()->eventPrint("You are not empowered to lead " + ob->GetName() + ".");
    return 1;
}
