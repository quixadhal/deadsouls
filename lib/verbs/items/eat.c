/*    /verbs/items/eat.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 951113
 *    Version: @(#) eat.c 1.3@(#)
 *    Last modified: 96/11/03
 */



#include <lib.h>
#include <rounds.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("eat");
    SetSynonyms("swallow");
    SetErrorMessage("Eat what?");
    SetRules("OBJ");
    SetHelp("Syntax: <eat FOOD>\n\n"
            "Allows you to munch on some food.  Keeping yourself well fed "
            "helps you heal faster from wounds and have greater stamina.\n\n"
            "See also: bait, cast, drink, fish");
}


void eventEat(object who, object what) {
    if( !what ) {
        return;
    }
    return what->eventEat(who);
}

mixed can_eat_obj() { 
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_eat_obj(object ob) {
    if( this_player()->GetInCombat() ) {
        this_player()->SetAttack(0, (: eventEat, this_player(), ob :),
                ROUND_OTHER);
    }
    else {
        eventEat(this_player(), ob);
    }
    return 1;
}
