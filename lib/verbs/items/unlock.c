/*    /verbs/items/unlock.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 951028
 *    Version: @(#) unlock.c 1.3@(#)
 *    Last modified: 97/01/02
 */

#include <lib.h>
#include "include/unlock.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("unlock");
    SetRules("OBJ with OBJ", "OBJ with STR");
    SetErrorMessage("Unlock what with what?");
    SetHelp("Syntax: <unlock OBJ with OBJ>\n\n"
            "The first object is something you wish to unlock, like a "
            "door or a chest.  The second is the key you wish to use to "
            "unlock it with.  If your key is the right key, then "
            "unlock allows you to unlock it.\n\n"
            "See also: close, lock, open, pick");
}

varargs mixed can_unlock_obj_with_obj(object foo, object bar,
        mixed words...) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

varargs mixed can_unlock_obj_with_str(mixed ob1, mixed ob2, mixed words...) {
    object wut = get_object(ob2);
    if(wut) ob2 = wut->GetKeyName();
    return can_unlock_obj_with_obj(ob1, ob2, words[1], ob2);
}

varargs mixed do_unlock_obj_with_obj(object ob1, object ob2, mixed args...) {
    return ob1->eventUnlock(this_player(), ob2);
}

varargs mixed do_unlock_obj_with_str(mixed ob1, mixed ob2, mixed words...) {
    return do_unlock_obj_with_obj(ob1, (get_object(lower_case(ob2)) || ""));
}

