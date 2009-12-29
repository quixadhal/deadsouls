#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(5);
    SetShort("underground passageway");
    SetLong("This is a dark underground passageway. The walls are rough and bare. To the west the passage opens up into a cave-like area. The passageway continues northeast and east from here.");
    SetItems(([
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "area", "cave like area", "cavelike area" }) : "It seems that to the west there is a n area like a cave which opens up to the outside.",
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ]));
    SetInventory(([
                "/domains/cave/npc/orc_guard5" : 1,
                ]));
    SetClimate("indoors");
    SetExits( ([
                "east" : "/domains/cave/room/cavepass2",
                "west" : "/domains/cave/room/cave",
                "northeast" : "/domains/cave/room/cavepass3.c",
                ]) );

}
void init(){
    ::init();
}
