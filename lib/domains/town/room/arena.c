#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the Arena");
    SetLong("You are in a large room with blank cement walls. "+
      "This room was built so Creators may test their armor, "+
      " weapons and NPC's in combat. There is a large metal door "+
      "on the west wall.");
    SetItems( ([
	({"wall","walls"}) : "The walls are smooth and cement.",
	({"floor","ceiling"}) : "The floor and ceiling are, like the walls, made "+
	"of smooth cement." ]) );
    SetExits( ([ "west" : "/domains/default/room/wiz_hall"
      ]) );
    SetDoor("west","/domains/town/doors/steel_door");
    SetInventory(([
	"/domains/town/npc/fighter" : 1,
	"/domains/town/npc/dummy" : 1,
	"/domains/town/obj/bbucket" :1
      ]));
    SetObviousExits("w");
    SetPlayerKill(1);
}

