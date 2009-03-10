#include <lib.h>
#include ROOMS_H
#include <respiration_types.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("An airless room");
    SetLong("A room whose only purpose is to have nothing to breathe.");
    SetRespirationType(R_VACUUM);
}

void init(){
    ::init();
}
