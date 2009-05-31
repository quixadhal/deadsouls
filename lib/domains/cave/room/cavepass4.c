#include <lib.h>
#include ROOMS_H

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(0);
    SetShort("underground passageway");
    SetLong("This is a dark underground passageway. The walls are rough and bare. The passageway contines north and south.");
    SetItems(([
                ({ "passage", "passageway", "This provides access to areas below the ground." }) : "This provides access to areas below the ground.",
                ({ "ground", "rock", "rocks", "earth", "dirt", "stone", "stones" }) : "Rough and dirty, as one would expect from a simple underground area.",
                ({ "wall", "walls", "rough walls", "rough and bare walls", "bare walls", "bare and rough walls" }) : "Carved roughly into the earth, this passageway has walls that expose underground rock. Clearly this area was architected with utility and expediency in mind.",
                ]));
    SetInventory(([
                "/domains/cave/npc/orc_guard4" : 2,
                ]));
    SetClimate("indoors");
    SetExits( ([ 
                "north" : "/domains/cave/room/cavepass5",
                "southeast" : "/domains/cave/room/cavepass3",
                ]) );

}
void init(){
    ::init();
    if(!find_object("/domains/cave/room/guardroom1")){
        catch(load_object("/domains/cave/room/guardroom1"));
    }
}
