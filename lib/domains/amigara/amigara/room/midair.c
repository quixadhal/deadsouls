#include <lib.h>
#include <terrain_types.h>
#include <medium.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    ::create();
    SetAmbientLight(5);
    SetShort("midair in a cave");
    SetLong("This is a cave in the stone mountain north of the town forest. There is no floor here. There is darkness below.");
    SetItems(([
                ({ "wall", "rocky wall", "mountain wall", "rough stone mountain wall", "stone mountain wall" }) : "Geological activity has created this cave ove the years, but clearly some sentient activity has formed it into a usable size and shape for habitation.",
                ]));
    SetExits( ([
                "south" : "/domains/amigara/room/cave",
                "down" : "/domains/amigara/room/passage1.c",
                ]) );

    SetInventory(([
                ]));
    SetClimate("indoors");
    SetTerrainType(T_MIDAIR);
    SetMedium(MEDIUM_AIR);
}
void init(){
    ::init();
}
