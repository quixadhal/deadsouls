/*    /verbs/rooms/enter.c
 *    from the Dead Soulsr2 Object Library
 *    created by Descartes of Borg 951104
 *    Version: @(#) enter.c 1.2@(#)
 *    Last modified: 96/10/15
 */
 
#include <lib.h>
#include "include/enter.h"
 
inherit LIB_VERB;
 
static void create() {
    verb::create();
    SetVerb("enter");
    SetRules("into OBJ", "OBJ");
    SetErrorMessage("Enter what?");
    SetHelp("Syntax: <enter PLACE>\n"
            "        <enter into PLACE>\n\n"
            "Moves you into the place you specify.  Whereas the \"go\" "
            "command is designed for motion towards something, the "
            "\"enter\" command is designed for motion into something.\n\n"
            "See also: go");
}
 
mixed can_enter_obj() {
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything.";
    }
    return 1;
}
 
mixed can_enter_into_obj() {
    return can_enter_obj();
}
 
mixed do_enter_obj(object targ, string what) {
    return targ->eventEnter(this_player(), remove_article(lower_case(what)));
}
 
mixed do_enter_into_obj(object targ, string what) {
    do_enter_obj(targ, what);
}
