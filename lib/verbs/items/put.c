/*    /verbs/items/put.c
 *    from the Dead Souls V Object Library
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
    SetRules("OBS in OBJ", "OBS into OBJ");
    SetErrorMessage("Put what into what?");
    SetHelp("Syntax: <put ITEM into CONTAINER>\n\n"
	    "Allows you to stick objects into other objects.\n\n"
	    "Synonyms: place, stick\n\n"
	    "See also: get, give, drop");
}

mixed can_put_obj_word_obj(object target, string wrd, object storage) {
    return eventCheckLight(this_player());
}

mixed do_put_obj_word_obj(object what, string wrd, object storage) {
    return (mixed)storage->eventPutInto(this_player(), what);
}

mixed do_put_obs_word_obj(mixed *res, string wrd, object storage) {
    object *obs;

    obs = filter(res, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

	ua = unique_array(res, (: $1 :));
	foreach(string *lines in ua) this_player()->eventPrint(lines[0]);
	return 1;
    }
    foreach(object ob in obs) storage->eventPutInto(this_player(), ob);
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
