/*    /daemon/distasters.c
 *    From Dead Souls LPMud
 *    Periodically searches for disasters that can happen to rooms
 *    Created by Descartes of Borg 961102
 *    Version: @(#) disasters.c 1.3@(#)
 *    Last modified: 96/11/02
 */

#include <lib.h>

inherit LIB_DAEMON;

void checkDisaster();

static void create() {
    daemon::create();
    SetNoClean(1);
    call_out((: checkDisaster :), 300);
}

static void checkDisaster() {
    object array rooms = objects((: $1->GetMedium() :));
    int chance = 100 - random(100) + 1;
    int count = 100; // check at most 100 rooms

    call_out((: checkDisaster :), 300);
    if( chance == 100 ) {
	return;
    }
    foreach(object room in rooms) {
	count--;
	if( room->eventDisaster(chance) ) {
	    return;
	}
	if( count < 1 ) {
	    return;
	}
    }
}
