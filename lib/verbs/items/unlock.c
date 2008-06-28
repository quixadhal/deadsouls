/*    /verbs/items/unlock.c
 *    from the Dead Souls Object Library
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
    SetRules("OBJ with OBJ");
    SetErrorMessage("Unlock what with what?");
    SetHelp("Syntax: <unlock OBJ with OBJ>\n\n"
      "The first object is something you wish to unlock, like a "
      "door or a chest.  The second is the key you wish to use to "
      "unlock it with.  If your key is the right key, then "
      "unlock allows you to unlock it.\n\n"
      "See also: close, lock, open, pick");
}

mixed can_unlock_obj_with_obj(mixed args...) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    //tc("can_unlock_obj_with_obj("+identify(args)+")");
    return this_player()->CanManipulate();
}

mixed do_unlock_obj_with_obj(mixed args...) {
    //tc("do_unlock_obj_with_obj("+identify(args)+")");
    return args[0]->eventUnlock(this_player(), args[1]);
}

