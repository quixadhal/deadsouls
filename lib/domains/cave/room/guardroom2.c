#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(0);
    SetShort("underground room");
    SetLong("This is a dark underground room. The walls are rough and bare. A passageway leads away to the south.");
    SetItems(([
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ]));
    SetInventory(([
                "/domains/cave/obj/tall_lamp" : 1,
                "/domains/cave/npc/orc_guard4" : 2,
                "/domains/cave/npc/orc_guard3" : 3,
                ]));
    SetExits( ([
                "south" : "/domains/cave/room/cavepass7",
                "north" : "/domains/cave/room/kurogane.c",
                ]) );
    SetClimate("indoors");

    SetDoor("north", "/domains/cave/doors/kurogane.c");

}
void init(){
    ::init();
}
