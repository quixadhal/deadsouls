#include <lib.h>
#include <rooms.h>

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(0);
    SetShort("underground passageway");
    SetLong("This is a dark underground passageway. The walls are rough and bare. The passageway runs southwest and northwest from here. There are rooms to the north, west, and south.");
    SetItems(([
        ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
        ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
        ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
      ]));
    SetClimate("indoors");
    SetExits( ([
        "north" : "/domains/cave/room/weapon_storage1",
        "west" : "/domains/cave/room/dining1",
        "northwest" : "/domains/cave/room/cavepass8",
        "southwest" : "/domains/cave/room/cavepass12",
        "south" : "/domains/cave/room/chieftain.c",
      ]) );

    SetInventory(([
      ]));
}
void init(){
    ::init();
}
