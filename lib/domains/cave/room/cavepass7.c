#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(0);
    SetShort("underground passageway");
    SetLong("This is a dark underground passageway. The walls are rough and bare. The passageway runs east and west. There are rooms to the north and south.");
    SetItems(([
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ]));
    SetClimate("indoors");
    SetExits( ([
                "north" : "/domains/cave/room/guardroom2",
                "south" : "/domains/cave/room/food_storage1",
                "west" : "/domains/cave/room/cavepass5",
                "east" : "/domains/cave/room/cavepass12.c",
                ]) );

    SetInventory(([
                ]));

    SetDoor("south", "/domains/cave/doors/food_storage1");

}
void init(){
    ::init();
}
