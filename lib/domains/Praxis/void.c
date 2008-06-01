#include <lib.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetShort("the void");
    SetLong("The void.  Go down to get out.");
    SetExits(([ "down": "/domains/Praxis/square" ]));
}
void init(){
    ::init();
}
