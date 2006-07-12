/*    /domains/Examples/etc/door.c
 *    from the Nightmare IV LPC Library
 *    an example door object connecting doorroom1.c and doorroom2.c
 *    created by Descartes of Borg 950411
 */

#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();
    SetSide("east", ([ "id" : ({ "gate","great gate" }) ,
	"short" : "the great gate leading east",
	"long" : "This is an imposing, massive, steel-barred gate leading east out of the fortress.",
	"lockable" : 1 ]) );
    SetSide("west", ([ "id" : ({ "gate","great gate" }),
	"short" : "the great gate leading west",
	"long" : "This is an imposing, massive, steel-barred gate leading west into the fortress.",
	"lockable" : 1 ]) );
    SetKeys("east", ({ "grand_gate_key" }));
    SetKeys("west", ({ "grand_gate_key" }));
    SetClosed(1);
    SetLocked(1);
}
