/*    /verbs/items/lock.c
 *    from the Dead Soulsr2 Object Library
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
    SetRules("OBJ with OBJ");
    SetErrorMessage("Lock what with what?");
    SetHelp("Syntax: <lock OBJ with OBJ>\n\n"
	    "The first object is something you wish to lock, like a "
	    "door or a chest.  The second is the key you wish to use to "
	    "lock it with.  If your key is the right key, then "
	    "lock allows you to lock it.\n\n"
	    "See also: close, open, pick, unlock");
}

mixed can_lock_obj_with_obj(string verb) {
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything.";
    }
    return 1;
}

varargs mixed do_lock_obj_with_obj(object target, object key, mixed *words...) {
    string id;

    id = remove_article(lower_case(words[0]));
    return (mixed)target->eventLock(this_player(), id, key);
}
