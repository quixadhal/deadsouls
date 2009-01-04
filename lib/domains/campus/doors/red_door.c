/*    /domains/Examples/etc/door.c
 *    from the Dead Souls LPC Library
 *    an example door object connecting doorroom1.c and doorroom2.c
 *    created by Descartes of Borg 950411
 */

#include <lib.h>

inherit LIB_DOOR;


static void create() {
    door::create();
    SetSide("red room", ([ "id" : ({"red door"}),
                "short" : "a red door",
                "long" : "This is a red door.",
                "lockable" : 1 ]) );
    SetSide("south", ([ "id" : ({"door","red door"}),
                "short" : "a red door",
                "long" : "This is a red door.",
                "lockable" : 1 ]) );
    SetClosed(1);
    SetLocked(1);
}
void init(){
    ::init();
}
