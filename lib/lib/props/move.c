/*    /lib/move.c
 *    From the Dead Souls Object Library
 *    Functions for basic movement
 *    Created by Descartes of Borg 950209
 *    Version: @(#) move.c 1.4@(#)
 *    Last modified: 96/12/31
 */

#include <message_class.h>

private static object LastEnvironment = 0;

// abstract methods
varargs void eventPrint(string str, mixed args...);
// end abstract methods

/* ******************  /lib/move.c attributes  ******************** */
object GetLastEnvironment() {
    return LastEnvironment;
}

/* ********************  /lib/move.c events  ********************** */
int eventMove(mixed dest) {
    object ob;

    if( !this_object() ) {
	return 0;
    }
    if( environment() ) {
	int x;

	x = (int)environment()->CanRelease(this_object());
        if( !x && !archp() ) {
	    return 0;
	}
    }
    if( stringp(dest) ) {
        if( !(ob = find_object(dest)) ) {
	    string str;

            if( str = catch(call_other(dest, "???")) ) {
                if( creatorp() ) {
		    eventPrint(str, MSG_ERROR);
		}
                return 0;
            }
            ob = find_object(dest);
        }
    }
    else {
	ob = dest;
    }
    if( !ob || ob == this_object() ) {
	return 0;
    }
    if( living(this_object()) && living(ob) ) {
	return 0;
    }
    if( !ob->CanReceive(this_object()) ) {
	return 0;
    }
    if( LastEnvironment = environment() ) {
	environment()->eventReleaseObject(this_object());
    }
    move_object(ob);
    environment()->eventReceiveObject(this_object());
    if( environment() ) {
	foreach(object peer in all_inventory(environment())) {
	    if( peer != this_object() ) {
		catch(peer->eventEncounter(this_object()));
	    }
	}
    }
    return (LastEnvironment != environment());
}
