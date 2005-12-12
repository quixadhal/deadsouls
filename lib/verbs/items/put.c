/*    /verbs/items/put.c
 *    from the Dead Souls Object Library
 *    put OBJ in OBJ
 *    put OBJ into OBJ
 *    created by Descartes of Borg 950114
 *    Version: @(#) put.c 1.2@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>
#include "include/put.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("put");
    SetSynonyms("place", "stick");
    SetRules("OBS in OBJ", "OBS into OBJ", "OBS on OBJ", "OBS onto OBJ");
    SetErrorMessage("Put what where?");
    SetHelp("Syntax: <put ITEM into CONTAINER>\n"
      "Syntax: <put ITEM onto SURFACE>\n\n"
      "Allows you to stick objects into other objects.\n\n"
      "Synonyms: place, stick\n\n"
      "See also: get, give, drop");
}

mixed can_put_obs_word_obj(mixed *res, string wrd, object storage) {
    if(wrd == "on" || wrd == "onto"){
	if(storage && !inherits( "/lib/comp/surface", storage ) ) return "#That isn't a load-bearing surface.";
    }
    if(storage && storage->GetClosed()){
	return "#That's closed.";
    }
    return eventCheckLight(this_player());
}

mixed can_put_obj_word_obj(object target, string wrd, object storage) {
    object *target_arr;
    if(target) target_arr = ({ target });
    return can_put_obs_word_obj(target_arr, wrd, storage);
}

mixed do_put_obj_word_obj(object what, string wrd, object storage) {

    if(wrd == "in" || wrd == "into") return (mixed)storage->eventPutInto(this_player(), what);
    if(wrd == "on" || wrd == "onto") return (mixed)storage->eventPutOnto(this_player(), what);
}

mixed do_put_obs_word_obj(mixed *res, string wrd, object storage) {
    object *obs;


    obs = filter(res, (: objectp :));
    if( !sizeof(obs) ) {
	mixed *ua;

	ua = unique_array(res, (: $1 :));
	write("That's not possible.");
	foreach(string *lines in ua) {
	    write("That doesn't seem possible at the moment.");
	    this_player()->eventPrint("That doesn't seem possible at the moment.");
	}
	return 1;
    }
    if(wrd == "in" || wrd == "into") {
	foreach(object ob in obs) storage->eventPutInto(this_player(), ob);
    }
    if(wrd == "on" || wrd == "onto") {
	foreach(object ob in obs) storage->eventPutOnto(this_player(), ob);
    }
    return 1;
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
