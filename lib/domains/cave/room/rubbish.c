#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(0);
    SetShort("underground garbage dump");
    SetLong("This underground area is full of foul piles of putrefying garbage and biological waste. A passageway leads west away from here.");
    SetItems(([
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ]));
    SetInventory(([
                "/domains/cave/obj/garbage" : 13,
                "/domains/cave/npc/rat" : 18,
                "/domains/cave/obj/waste" : 37,
                ]));
    SetClimate("indoors");
    SetExits(([
                "west" : "/domains/cave/room/cavepass2",
                ]));

}
void init(){
    ::init();
}
