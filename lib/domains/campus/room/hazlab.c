#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Hazardous Technologies Lab");
    SetLong("The University's research often yields mysterious, advanced, or alien artifacts which may pose a hazard to normal people. This laboratory is where such items are brought for analysis.");
    SetInventory(([
                "/domains/campus/npc/turret" : 1,
                "/domains/campus/obj/workbench" : 1,
                "/domains/default/vehicle/hoverpod" : 1,
                ]));
    SetExits(([
                "north" : "/domains/campus/room/science2",
                ]));

    SetClimate("indoors");

}
void init(){
    ::init();
}
