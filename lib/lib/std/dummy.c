/*    /lib/obj/dummy.c
 *    From the Dead Souls V Object Library
 *    A dummy item object that handles item descriptions and such
 *    Created by Descartes of Borg 961014
 *    Version: @(#) dummy.c 1.18@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>
#include <function.h>

inherit LIB_CLEAN;
inherit LIB_DESCRIPTION;
inherit LIB_ENTER;
inherit LIB_ID;
inherit LIB_LISTEN;
inherit LIB_LOOK;
inherit LIB_RADIANCE;
inherit LIB_READ;
inherit LIB_SEARCH;
inherit LIB_SMELL;
inherit LIB_TOUCH;

/* ***************** dummy.c attributes ***************** */
int isDummy() {
    return 1;
}

varargs string array SetId(mixed ids...) {
    ids = id::SetId(ids);
    if( sizeof(ids) && !GetKeyName() ) {
	SetKeyName(ids[0]);
	if( !GetShort() ) {
	    SetShort(add_article(ids[0]));
	}
    }
    return ids;
}

varargs int GetInvis(object ob) {
    return 1;
}

/* ********************* dummy.c events ****************** */
static int Destruct() {
    object env;
    int x;

    if( env = environment() ) {
        env->eventReleaseObject(this_object());
	x = clean::Destruct();
	if( !x ) {
	    env->eventReceiveObject(this_object());
	}
        return x;
    }
    else return clean::Destruct();
}

mixed eventMove(mixed dest) {
    object ob;

    if( stringp(dest) ) {
	ob = load_object(dest);
    }
    else {
	ob = dest;
    }
    move_object(ob);
    if( environment() != ob ) {
	return 0;
    }
    else {
	return 1;
    }
}
    
/* ******************* dummy.c driver applies ******************** */
varargs static void create(string array id, mixed long, string array adj) {
    enter::create();
    parse_init();
    if( adj ) {
	SetAdjectives(adj...);
    }
    if( id ) {
	SetId(id...);
    }
    SetExternalDesc(long || "");
}
