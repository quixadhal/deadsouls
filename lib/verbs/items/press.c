/*    /verbs/items/press.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960114
 *    Version: @(#) press.c 1.3@(#)
 *    Last modified: 96/10/18
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("press");
    SetRules("OBJ", "STR on OBJ");
    SetSynonyms("push", "poke", "prod");
    SetErrorMessage("Press what? Or press what on what?");
    SetHelp("Syntax: <press OBJECT>\n"
	    "        <press THING on OBJECT>\n\n"
	    "Allows you to press an object, or perhaps a thing on the "
	    "object.  For example, you might want to "
	    "\"press the button on the wall\", or simply \"press button\".\n\n"
	    "Synonyms: poke, prod, push");
}

mixed can_press_obj() {
    return 1;
}

mixed can_press_str_on_obj() {
    return 1;
}

mixed do_press_obj(object target) {
    return target->eventPress(this_player());
}

varargs mixed do_press_str_on_obj(string thing, object target) {
    return target->eventPress(this_player(), remove_article(lower_case(thing)));
}
