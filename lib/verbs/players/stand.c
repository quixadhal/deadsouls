/*    /verbs/players/stand.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960711
 *    Version: @(#) stand.c 1.5@(#)
 *    Last Modified: 96/12/21
 */

#include <lib.h>
#include <position.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("stand");
    SetRules("", "up");
    SetErrorMessage("Stand up?");
    SetSynonyms("get");
    SetHelp("Syntax: <stand up>\n"
	    "When sitting down or lying down, you can get up in this "
	    "most intuitive manner.\n\n"
	    "See also: lie, sit");
}

mixed can_stand_up() {
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything.";
    }
    if( this_player()->GetPosition() != POSITION_STANDING ) {
	return 1;
    }
    return "You are already standing up!";
}

mixed do_stand_up() {
    return (mixed)this_player()->eventStand();
}
