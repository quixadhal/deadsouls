/*    /verbs/items/touch.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 961014
 *    Version: @(#) touch.c 1.1@(#)
 *    Last modified: 96/10/15
 */

#include <lib.h>
#include <daemons.h>
#include <function.h>
 
inherit LIB_VERB;
 
static void create() {
    verb::create();
    SetVerb("touch");
    SetRules("OBJ", "STR on OBJ", "STR of OBJ");
    SetErrorMessage("Touch something?");
    SetHelp("Syntax: <touch ITEM>\n"
	    "        <touch THING on ITEM>\n\n"
	    "This command allows you to touch an object to get an idea "
	    "of any special textural properties it may have.\n\n"
	    "See also: listen, look, read, search, smell");
}
 
mixed can_touch_obj() {
    return 1;
}


mixed can_touch_str_word_obj() {
    return 1;
}

mixed do_touch_obj(object ob) {
    return ob->eventTouch(this_player());
}

mixed do_touch_str_word_obj(string str, object ob) {
    return ob->eventTouch(this_player(), str);
}
