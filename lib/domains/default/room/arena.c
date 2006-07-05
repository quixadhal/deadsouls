#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the Arena");
    SetLong("You are in a large room with blank cement walls. This room was built so Creators may test their armor, weapons and NPC's in combat. A large steel door is here, which can be used to prevent wimpy creatures from escaping.");
    SetItems( ([
	({"wall","walls"}) : "The walls are smooth and cement.",
	({"floor","ceiling"}) : "The floor and ceiling are, like the walls, made "+
	"of smooth cement." ]) );
    SetExits( ([ 
	"south" : "/domains/default/room/wiz_corr1",
      ]) );
    SetInventory(([
	"/domains/default/npc/fighter" : 1,
	"/domains/default/npc/dummy" : 1,
	"/domains/default/obj/bbucket" :1
      ]));
    SetPlayerKill(1);

    SetDoor("south", "/domains/default/doors/steel_door2.c");

}
void init(){
    ::init();
}
