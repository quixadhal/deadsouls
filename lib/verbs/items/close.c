/*    /verbs/items/close.c
 *    from the Dead Souls V Object Library
 *    close OBJ
 *    created by Descartes of Borg 960115
 *    Version: @(#) close.c 1.2@(#)
 *    Last modified: 96/10/15
 */

#include <lib.h>
#include "include/close.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("close");
    SetRules("OBJ");
    SetSynonyms("shut");
    SetErrorMessage("Close what?");
    SetHelp("Syntax: <close OBJECT>\n\n"
	    "Closes a door or chest or some other such object.\n\n"
	    "Synonyms: shut\n\n"
	    "See also: lock, open, pick, unlock");
}

mixed can_close_obj(string verb) {
    if( this_player()->GetParalyzed() ) {
	return "You are unable to do anything.";
    }
    return 1;
}

varargs mixed do_close_obj(object ob, mixed *args...) {
    string what;

    what = remove_article(lower_case(args[0]));
    return (mixed)ob->eventClose(this_player(), what);
}
