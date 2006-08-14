#include <lib.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetProperties(([ "light" : 3, "no magic" : 1 ]));
    SetShort("the underworld");
    SetLong("This will become something more interesting.  Exit is up.");
    AddExit("up", "/domains/Praxis/square");
}
