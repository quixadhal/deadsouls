#include <lib.h>
#include "/domains/fort/customdefs.h"
#include <position.h>
inherit  LIB_SENTIENT;
void create(){
    ::create();
    SetKeyName("dude");
    SetId("guy");
    SetShort("a dude");
    SetLong("This is The Dude.");
    SetAutoStand(0);
    SetInventory(([
	"/domains/fort/weap/beads3" : "wield beads",
	"/domains/fort/obj/key" : 1,
      ]));
    SetLevel(5);
    SetPosition(POSITION_STANDING);
    SetGender("male");
    SetRace("human");
}

void init() {
    ::init();
    SetSmell(([  "default" : "A rather odd smell."]));
    SetListen(([  "default" : "Nobody listens to the dude."]));
}