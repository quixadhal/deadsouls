/*    /verbs/items/listen.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 961014
 *    Version: @(#) listen.c 1.1@(#)
 *    Last modified: 96/10/14
 */
 
#include <lib.h>
#include <daemons.h>
#include <function.h>
 
inherit LIB_VERB;
 
static void create() {
    verb::create();
    SetVerb("listen");
    SetRules("", "OBJ", "to OBJ", "to STR on OBJ", "to STR of OBJ");
    SetErrorMessage("Listen to something?");
    SetHelp("Syntax: <listen>\n"
            "        <listen to ITEM>\n\n"
	    "Without any arguments, this command allowed you to listen "
	    "to your general surroundings.  You may, however, concentrate "
	    "your listening on any target.  If it is making noise and you "
	    "can hear well, then you will hear any relevant noise it is "
	    "making.\n\n"
	    "See also: look, search, smell, touch");
}
 
mixed can_listen() {
    if( !environment(this_player()) ) {
	return "You are nowhere.";
    }
    return 1;
}

mixed can_listen_obj() {
    return 1;
}

mixed can_listen_to_obj() {
    return 1;
}

mixed can_listen_to_str_word_obj() {
    return 1;
}

mixed do_listen() {
    mixed val = environment(this_player())->GetListen();

    if( functionp(val) ) {
	if( !(functionp(val) & FP_OWNER_DESTED) ) {
	    val = evaluate(val, this_player());
	}
	else {
	    val = 0;
	}
    }
    if( !val ) {
	val = "You don't hear a thing.";
    }
    environment(this_player())->eventPrint(this_player()->GetName() +
                                           " puts an ear to the air.",
					   this_player());
    this_player()->eventPrint(val);
    return 1;
}

mixed do_listen_obj(object ob) {
    return ob->eventListen(this_player());
}

mixed do_listen_to_obj(object ob) {
    return do_listen_obj(ob);
}
 
mixed do_listen_to_str_word_obj(string str, object ob) {
    return ob->eventListen(this_player(), str);
}
