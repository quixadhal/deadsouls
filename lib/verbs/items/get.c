/*    /verbs/items/get.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 950113
 *    Version: @(#) get.c 1.5@(#)
 *    Last Modified: 96/11/12
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("get");
    SetRules("WRD WRD from OBJ", "WRD WRD out of OBJ", "OBS",
	     "OBS out of OBJ", "OBS from OBJ");
    SetSynonyms("take");
    SetErrorMessage("Get what?  Or perhaps get something from somewhere?");
    SetHelp("Syntax: <get ITEM>\n"
	    "        <get ITEM from OBJECT>\n"
	    "        <get ITEM out of OBJECT>\n"
	    "        <get AMOUNT CURRENCY from pile>\n"
	    "        <get AMOUNT CURRENCY out of pile>\n\n"
	    "This allows you to get items in the same room as you, or "
	    "contained inside other items so that you are carrying them.  "
	    "In addition, you can specify partial amounts of currency to "
	    "pick up from a pile lying around.\n\n"
	    "Synonyms: take\n\n"
	    "See also: drop, give, put");
}

mixed eventCheckLight(object who) {
    int light;
    
    if( (light = effective_light(who)) < 0 ) {
	if( 100 + (10*light) < random(100) )
	  return "You fumble around in the darkness.";
	else return 1;
    }
    else if( light > 4 ) {
	if( 100 - (10*light) < random(100) )
	  return "You fumble around in the blinding light.";
	else return 1;
    }
    else return 1;
}

mixed can_get_obj(string verb) {
    return eventCheckLight(this_player());
}

mixed can_get_obj_out_of_obj(string verb) {
    return eventCheckLight(this_player());
}

mixed can_get_obj_from_obj(string verb) {
    return can_get_obj_out_of_obj(verb);
}

mixed can_get_wrd_wrd_out_of_obj(string num, string curr) {
    return eventCheckLight(this_player());
}

mixed can_get_wrd_wrd_from_obj(string num, string curr) {
    return can_get_wrd_wrd_out_of_obj(num, curr);
}

mixed do_get_obj(object ob) {
    return ob->eventGet(this_player());
}

mixed do_get_wrd_wrd_out_of_obj(string num, string curr, object pile) {
    return pile->eventGetMoney(this_player(), to_int(num), curr);
}

mixed do_get_wrd_wrd_from_obj(string num, string curr, object pile) {
    return do_get_wrd_wrd_out_of_obj(num, curr, pile);
}

mixed do_get_obj_out_of_obj(object ob, object storage) {
    return (mixed)storage->eventGetFrom(this_player(), ({ ob }));
}

mixed do_get_obj_from_obj(object ob, object storage) {
    return do_get_obj_out_of_obj(ob, storage);
}

mixed do_get_obs(mixed *targs) {
    object *obs;

    if( !sizeof(targs) ) {
        this_player()->eventPrint("There is no such thing to be taken.");
	return 1;
    }
    obs = filter(targs, (: objectp :));
    if( !sizeof(obs) ) {
	mapping messages = unique_mapping(targs, (: $1 :));
	
	foreach(string msg in keys(messages)) {
	    this_player()->eventPrint(msg);
	}
	return 1;
    }
    foreach(object item in obs) {

	if( (mixed)item->CanGet(this_player()) != 1 ) continue;
	item->eventGet(this_player());
    }
    return 1;
}

mixed do_get_obs_out_of_obj(mixed *targs, object storage) {
    object *obs;

    if( !sizeof(targs) ) {
        this_player()->eventPrint("There is no such thing to be taken.");
	return 1;
    }
    obs = filter(targs, (: objectp :));
    if( !sizeof(obs) ) {
	mapping messages = unique_mapping(targs, (: $1 :));

	foreach(string msg in keys(messages)) {
	    this_player()->eventPrint(msg);
	}
	return 1;
    }
    return (mixed)storage->eventGetFrom(this_player(), obs);
}

mixed do_get_obs_from_obj(mixed *obs, object storage) {
    return do_get_obs_out_of_obj(obs, storage);
}
