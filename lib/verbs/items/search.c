/*    /verbs/items/search.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960121
 *    Version: @(#) search.c 1.2@(#)
 *    Last modified: 96/10/15
 */

#include <lib.h>
#include <daemons.h>
#include <function.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("search");
    SetRules("", "OBJ", "STR on OBJ", "STR of OBJ");
    SetErrorMessage("Search something?");
    SetHelp("Syntax: <search>\n"
            "        <search ITEM>\n"
            "        <search THING on ITEM>\n\n"
            "Without any arguments, this command allows you to search "
            "your general surroundings.  You may, however, concentrate "
            "your searching on any target.\n\n"
            "See also: listen, look, read, smell, touch");
}

mixed can_search() {
    if( !environment(this_player()) ) {
        return "You are nowhere.";
    }
    return 1;
}

mixed can_search_obj() {
    return 1;
}


mixed can_search_str_word_obj() {
    return 1;
}

mixed do_search() {
    mixed val = environment(this_player())->GetSearch();

    if( functionp(val) ) {
        if( !(functionp(val) & FP_OWNER_DESTED) ) {
            val = evaluate(val, this_player());
        }
        else {
            val = 0;
        }
    }
    if( !val ) {
        val = "You don't find a thing.";
    }
    environment(this_player())->eventPrint(this_player()->GetName() +
            " searches around.", this_player());
    this_player()->eventPrint(val);
    return 1;
}

mixed do_search_obj(object ob) {
    return ob->eventSearch(this_player());
}

mixed do_search_str_word_obj(string str, object ob) {
    return ob->eventSearch(this_player(), str);
}
