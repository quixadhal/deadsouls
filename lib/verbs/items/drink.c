/*    /verbs/items/drink.c
 *    from the Dead Souls Mud Library
 *    drink
 *    drink OBJ
 *    created by Descartes of Borg 951113
 */



#include <lib.h>
#include <rounds.h>
#include "include/drink.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("drink");
    SetRules("OBJ", "from OBJ");
    SetErrorMessage("Drink what?");
    SetHelp("Syntax: <drink LIQUID>\n"
            "        <drink from THING>\n\n"
            "Allows you to drink a liquid substance.   Often these "
            "substances come in the form of caffeine, alcohol, and just "
            "regular drinks.  Different types of drinks have different "
            "effects on you which you will learn through experimentation.\n\n"
            "See also: bait, cast, eat, fish");
    SetSynonyms("quaff");
}

mixed can_drink_obj(string verb) { return this_player()->CanManipulate(); }

mixed can_drink_from_obj(string verb) { return this_player()->CanManipulate(); }

    mixed do_drink_obj(object ob) {
        if( this_player()->GetInCombat() )
            this_player()->SetAttack(0, (: eventDrink, this_player(), ob :),
                    ROUND_OTHER);
        else eventDrink(this_player(), ob);
        return 1;
    }

    mixed do_drink_from_obj(object ob, string id) {
        if( this_player()->GetInCombat() )
            this_player()->SetAttack(0, (: eventDrink, this_player(), ob, id :),
                    ROUND_OTHER);
        else eventDrink(this_player(), ob, id);
        return 1;
    }

varargs void eventDrink(object who, object what, string id) {
    return what->eventDrink(who, id);
}
