/*    /verbs/items/ring.c
 *    From the Dead Souls V Object Library
 *    Created by Descartes of Borg 961017
 *    Version: @(#) ring.c 1.1@(#)
 *    Last modified: 96/10/17
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("ring");
    SetRules("OBJ", "STR on OBJ", "OBJ with OBJ", "STR on OBJ with OBJ");
    SetErrorMessage("Ring what?");
    SetHelp("Syntax: <ring OBJECT>\n"
	    "        <ring OBJECT on OBJECT>\n"
	    "        <ring OBJECT with OBJECT>\n"
	    "        <ring OBJECT on OBJECT with OBJECT>\n\n"
	    "Allows you to ring things that ring.");
}

mixed can_ring_obj() {
    if( this_player()->GetParalized() > 0 ) {
	return "You cannot do anything.";
    }
    return 1;
}

mixed can_ring_str_on_obj() {
    return can_ring_obj();
}

mixed can_ring_obj_with_obj() {
    return can_ring_obj();
}

mixed can_ring_str_on_obj_with_obj() {
    return can_ring_obj();
}

mixed do_ring_obj(object target) {
    return target->eventRing(this_player());
}

mixed do_ring_str_on_obj(string str, object target) {
    return target->eventRing(this_player(), remove_article(lower_case(str)));
}

mixed do_ring_obj_with_obj(object target, object tool) {
    return target->eventRing(this_player(), 0, tool);
}

mixed do_ring_str_on_obj_with_obj(string str, object target, object tool) {
    return target->eventRing(this_player(), remove_article(lower_case(str)),
			     tool);
}
