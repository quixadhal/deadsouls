/*    /domains/Examples/etc/door.c
 *    from the Dead Souls LPC Library
 *    an example door object connecting doorroom1.c and doorroom2.c
 *    created by Descartes of Borg 950411
 */

#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();
    SetSide("north", (["id" : ({"door","stairwell door"}),
                "short" : "the stairwell door",
                "long" : "This is a metal door leading out of the stairwell.",
                "lockable" : 0 ]) );
    SetSide("south", (["id" : ({"door","stairwell door"}),
                "short" : "the stairwell door",
                "long" : "This is a metal door leading south into a stairwell.",
                "lockable" : 0 ]) );
    SetClosed(1);
}
