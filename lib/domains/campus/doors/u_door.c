/*    /domains/Examples/etc/door.c
 *    from the Dead Souls LPC Library
 *    an example door object connecting doorroom1.c and doorroom2.c
 *    created by Descartes of Borg 950411
 */

#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();
    SetSide("east", ([ "id" : "door",
                "short" : "a door leading east",
                "long" :  "This is steel-plated, otherwise perfectly normal door.",
                "lockable" : 0 ]));
    SetSide("west",(["id":"door",
                "short" : "a door leading west",
                "long":"This is steel-plated, otherwise perfectly normal door.",
                "lockable" : 0 ]));
    SetClosed(1);
}
