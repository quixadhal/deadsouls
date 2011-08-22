#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Learning Center Start");
    SetLong("This is where you can start learning how to interact with the virtual world you've connected to.\n\nTo begin the tutorial, enter: tutorial\n\nTo return to the normal areas, enter: normal\n");
    SetCoordinates("2000,2000,0");
    SetNoObviousExits(1);
    SetNoClean(1);
}
void init(){
    ::init();
}
