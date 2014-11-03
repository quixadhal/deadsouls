#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    ::create();
    SetAmbientLight(5);
    SetShort("a dark passageway");
    SetLong("This is a dark underground passageway. The walls are rough and bare.");
    SetItems(([
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "area", "cave like area", "cavelike area" }) : "A space torn or carved into the mountain.",
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ]));
    SetExits( ([
                "east" : "/domains/amigara/room/passage14",
                "north" : "/domains/amigara/room/passage18.c",
                ]) );

    SetInventory(([
                ]));
    SetClimate("indoors");

}
void init(){
    ::init();
}
