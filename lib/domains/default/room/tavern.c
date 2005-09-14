#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Local Tavern");
    SetLong("You are in the local pub. You can order drinks and a sandwich here:\n\n"+
		    "\tFirst class ale:\t\t\t\t10 dollars\n"+
		    "\tSpecial of the house:\t\t\t\t20 dollars\n"+
		    "\tFirebreather:\t\t\t\t\t50 dollars\n"+
		    "\tSpring water:\t\t\t\t\t2 dollars\n"+
		    "\tRegular coffee:\t\t\t\t\t5 dollars\n"+
		    "\tImported espresso:\t\t\t\t15 dollars\n"+
		    "\tHam sandwich:\t\t\t\t\t10 dollars\n\n\n"+
		    "To order a bottle of water, \"buy water from lars\"\n");
		SetExits( ([
	                      "west" : "/domains/default/room/shop",
	                     ]) );
SetObviousExits("w");
SetProperty("no attack", 1);		    
SetInventory(([
//"/domains/default/npc/lars" :1
]) );
}
