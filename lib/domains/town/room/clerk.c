#include <lib.h>
inherit LIB_CHAPEL;

static void create() {
    chapel::create();
    SetTown("town");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Town Clerk");
    SetLong("This small office is where people come to conduct their official public business. The main area of the town hall is south.");
    SetProperties (([
                "no attack":1, 
                "no bump":1, 
                "no steal":0, 
                "no magic":0, 
                "no paralyze":0, 
                "no teleport":0]));
    SetItems(([]));
    SetExits( ([ 
                "south" : "/domains/town/room/thall",
                ]) );
    SetInventory(([
                "/domains/town/npc/clerk" : ({60, 1}),
                ]));
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
