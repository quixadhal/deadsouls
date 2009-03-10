/*    /verbs/items/smell.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960121
 *    Version: @(#) smell.c 1.2@(#)
 *    Last modified: 96/10/15
 */

#include <lib.h>
#include <daemons.h>
#include <function.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("smell");
    SetRules("", "OBJ", "STR on OBJ", "STR of OBJ");
    SetErrorMessage("Smell something?");
    SetHelp("Syntax: <smell>\n"
            "        <smell ITEM>\n"
            "        <smell THING on ITEM>\n\n"
            "Without any arguments, this command allows you to smell "
            "your general surroundings.  You may, however, concentrate "
            "your smelling on any target.\n\n"
            "See also: listen, look, read, search, touch");
}

mixed can_smell() {
    if( !environment(this_player()) ) {
        return "You are nowhere.";
    }
    return 1;
}

mixed can_smell_obj() {
    return 1;
}


mixed can_smell_str_word_obj() {
    return 1;
}

mixed do_smell() {
    mixed val = environment(this_player())->GetSmell();

    if( functionp(val) ) {
        if( !(functionp(val) & FP_OWNER_DESTED) ) {
            val = evaluate(val, this_player());
        }
        else {
            val = 0;
        }
    }
    if( !val ) {
        val = "You don't smell a thing.";
    }
    environment(this_player())->eventPrint(this_player()->GetName() +
            " smells around.", this_player());
    this_player()->eventPrint(val);
    return 1;
}

mixed do_smell_obj(object ob) {
    return ob->eventSmell(this_player());
}

mixed do_smell_str_word_obj(string str, object ob) {
    return ob->eventSmell(this_player(), str);
}
