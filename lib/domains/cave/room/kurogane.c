#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(0);
    SetShort("commander's quarters");
    SetLong("This is a dark underground room. The walls are rough and bare. A passageway leads away to the south.");
    SetItems(([
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ]));
    SetInventory(([
                "/domains/cave/npc/kurogane" : 1,
                "/domains/cave/obj/tall_lamp" : 1,
                "/domains/cave/obj/table" : 1,
                "/domains/cave/obj/chest" : 1,
                "/domains/cave/obj/cot" : 1,
                ]));
    SetExits(([
                "south" : "/domains/cave/room/guardroom2",
                ]));

    SetClimate("indoors");

    SetDoor("south", "/domains/cave/doors/kurogane.c");

}
void init(){
    ::init();
}
