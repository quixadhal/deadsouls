/*    /lib/comp/object.c
 *    From the Dead Souls LPC Library
 *    Object inherited by all tangible objects (ones with environments)
 *    Created by Descartes of Borg 940211
 *    Version: @(#) object.c 1.10@(#)
 *    Last Modified: 96/12/22
 */

#include <lib.h>

inherit LIB_CLEAN;
inherit LIB_DESCRIPTION;
inherit LIB_ID;
inherit LIB_LISTEN;
inherit LIB_LOOK;
inherit LIB_PROPERTIES;
inherit LIB_RADIANCE;
inherit LIB_SEARCH;
inherit LIB_SMELL;
inherit LIB_TOUCH;

/* ************************ object.c events ********************* */
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

/* ******************* object.c driver applies **************** */
static void create() {
    parse_init();
}

