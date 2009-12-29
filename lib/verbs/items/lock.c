/*    /verbs/items/lock.c
 *    from the Dead Souls Mud Library
 *    lock OBJ with OBJ
 *    created by Descartes of Borg 951028
 *    Version: @(#) lock.c 1.2@(#)
 *    Last modified: 96/10/15
 */

#include <lib.h>
#include "include/lock.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("lock");
    SetRules("OBJ with OBJ","OBJ with STR");
    SetErrorMessage("Lock what with what?");
    SetHelp("Syntax: <lock OBJ with OBJ>\n\n"
            "The first object is something you wish to lock, like a "
            "door or a chest.  The second is the key you wish to use to "
            "lock it with.  If your key is the right key, then "
            "lock allows you to lock it.\n\n"
            "See also: close, open, pick, unlock");
}

varargs mixed can_lock_obj_with_obj(mixed ob1, mixed ob2,
        mixed mix1, mixed mix2) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

varargs mixed can_lock_obj_with_str(mixed ob1, mixed ob2, mixed words...) {
    object wut = get_object(ob2);
    if(wut) ob2 = wut->GetKeyName();
    return can_lock_obj_with_obj(0, 0, words...);
}

varargs mixed do_lock_obj_with_obj(object target, object key, mixed words...) {
    string id;
    id = remove_article(lower_case(words[0]));
    return target->eventLock(this_player(), id, key);
}

varargs mixed do_lock_obj_with_str(mixed ob1, mixed ob2, mixed words...) {
    return do_lock_obj_with_obj(ob1, (get_object(lower_case(ob2)) || ""));
}
