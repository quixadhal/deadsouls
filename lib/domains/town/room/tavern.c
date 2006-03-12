#include <lib.h>
inherit LIB_ROOM;

static void create() {
    ::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Local Tavern");
    SetLong("You are in the local pub. You can order drinks and a sandwich here:\n\n"+
      "\tFirst class ale:\t\t\t\t10 silver\n"
      "\tSpecial of the house:\t\t\t\t20 silver\n"
      "\tFirebreather:\t\t\t\t\t50 silver\n"
      "\tSpring water:\t\t\t\t\t2 silver\n"
      "\tRegular coffee:\t\t\t\t\t5 silver\n"
      "\tImported espresso:\t\t\t\t15 silver\n"
      "\tHam sandwich:\t\t\t\t\t10 silver\n\n\n"
      "To order a bottle of water, \"buy water from lars\"\n"
    );
    SetInventory(([
	"/domains/town/obj/bbucket" :1,
	"/domains/town/npc/lars" :1
      ]) );
    SetExits( ([
	"west" : "/domains/town/room/road",
	"out" : "/domains/town/room/road",
      ]) );
    SetProperty("no attack", 1);		    
}
void init(){
    ::init();
}