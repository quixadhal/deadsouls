/*    /verbs/items/open.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960115
 *    Version: @(#) open.c 1.2@(#)
 *    Last modified: 96/10/15
 */

#include <lib.h>
#include "include/open.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("open");
    SetRules("OBJ", "OBJ with OBJ");
    SetErrorMessage("Open what? Or open what with what?");
    SetHelp("Syntax: <open OBJECT>\n"
            "        <open OBJECT with ITEM>\n\n"
            "Opens a door or chest or some other such object.  Some things "
            "may be opened by force using items capable of forcing things "
            "to open.\n\n"
            "See also: close, lock, pick, unlock");
}

mixed can_open_obj(string verb, string id) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed can_open_obj_with_obj(string verb, string id1, string id2) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

varargs mixed do_open_obj(object ob, mixed *args...) {
    string what;

    what = remove_article(lower_case(args[0]));
    return ob->eventOpen(this_player(), what);
}

varargs mixed do_open_obj_with_obj(object ob, object with, mixed *args...) {
    string what;

    what = remove_article(lower_case(args[0]));
    return ob->eventOpen(this_player(), what, with);
}
