#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(0);
    SetShort("post commissary");
    SetLong("This is a dark underground room with rough walls. It is outfitted to accommodate personnel who want to eat and drink. Passageways lead north, east, and south from here.");
    SetItems(([
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ]));
    SetInventory(([
                "/domains/cave/obj/tall_lamp" : 1,
                "/domains/cave/obj/chair" : 6,
                "/domains/cave/obj/table2" : 3,
                "/domains/cave/obj/rack" : 1,
                "/domains/cave/obj/lister" : 1,
                "/domains/cave/obj/cask5" : 1,
                "/domains/cave/npc/rybak" : 1,
                ]));
    SetClimate("indoors");
    SetExits( ([
                "south" : "/domains/cave/room/cavepass12",
                "east" : "/domains/cave/room/cavepass9",
                "north" : "/domains/cave/room/cavepass8.c",
                ]) );

}
void init(){
    ::init();
}
