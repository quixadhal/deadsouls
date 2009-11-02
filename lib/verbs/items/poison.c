/*  /verbs/items/poison.c
 *  from the Dead Souls  Library
 *  poison
 *  poison OBJ with OBJ
 *  created by Blitz@Dead Souls 951020
 *  a check for poison uses put in by BillGates 961202
 */



#include <lib.h>
#include <rounds.h>
#include "include/poison.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("poison");
    SetRules("OBJ with OBJ");
    SetErrorMessage("Poison what with what?");
    SetHelp("Syntax: <poison ITEM with POISON>\n\n"
            "This command allows you to poison items, provided you "
            "have a poisoning agent, and something to poison.  The "
            "ITEM is the item which you want to poison.  The "
            "POISON is what you wish to use to poison it with.");
}

mixed can_poison_obj_with_obj(string verb) { return this_player()->CanManipulate(); }

    mixed do_poison_obj_with_obj(object target, object agent) {
        if( this_player()->GetInCombat() )
            this_player()->SetAttack(0, (: eventPoison, this_player(), target,
                        agent :), ROUND_OTHER);
        else eventPoison(this_player(), target, agent);
        return 1;
    }

void eventPoison(object who, object target, object agent) {
    if( !who ) return;
    if( environment(agent) != this_player() ) {
        who->eventPrint("You no longer have your poison.");
        return;
    }
    if( !(agent->GetPoisonUses()) ) {
        who->eventPrint("Your poison is all used up.");
        return;
    }
    agent->eventSpreadPoison(who, target);
}
