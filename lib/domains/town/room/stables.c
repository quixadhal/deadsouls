#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Village Stables");
    SetLong("These are the village stables. Townsfolk come here and leave their mounts in the care of Ingrid, the local horse girl. The place is orderly, if a bit stinky, and it's clear Ingrid runs a professional outfit.");
    SetItems(([
                ({ "building", "schoolhouse", "school", "village school", "village schoolhouse" }) : "A small but well-kept wooden building where townsfolk and their children go to acquire knowledge.",
                ({ "building", "buildings" }) : "Structures designed for human occupancy.",
                ]));
    SetProperties(([
                "no attack" : 1,
                ]));
    SetInventory(([
                "/domains/town/npc/ingrid" : 1,
                "/domains/town/npc/horse" : 1,
                ]));
    SetExits(([
                "north" : "/domains/town/room/vill_road4",
                ]));

    SetEnters( ([ 
                ]) );

}
void init(){
    ::init();
}
