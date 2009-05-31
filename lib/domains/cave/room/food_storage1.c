#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(0);
    SetShort("underground room");
    SetLong("This is a dark underground room. The walls are rough and bare. A passageway leads away to the north.");
    SetItems(([
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "room", "here", "area" }) : "Carved roughly into the earth, this area has walls that expose underground rock.",
                ]));
    SetInventory(([
                "/domains/cave/obj/fishbarrel" : 1,
                "/domains/cave/obj/cask4" : 1,
                "/domains/cave/obj/cask3" : 1,
                "/domains/cave/obj/cask2" : 2,
                "/domains/cave/obj/cask1" : 3,
                "/domains/cave/obj/jerkybox" : 3,
                "/domains/cave/obj/jerkybox2" : 1,
                ]));
    SetClimate("indoors");
    SetExits(([
                "north" : "/domains/cave/room/cavepass7",
                ]));

    SetDoor("north", "/domains/cave/doors/food_storage1");

}
void init(){
    ::init();
}
