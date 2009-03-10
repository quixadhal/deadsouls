#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("the secret stash");
    SetLong("This appears to be a secret room for saving valuables. The "+
            "master bedroom is west.");
    SetExits( ([ "west" : "/domains/town/room/mansion_mbdroom" ]) );
    SetNoClean(1);
    SetPersistent(1);
    RestoreObject();
}

void init(){
    ::init();
}
