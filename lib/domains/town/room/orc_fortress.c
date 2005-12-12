#include <lib.h>
inherit LIB_ROOM;

int PreExit(){
    object *livings;
    livings = get_livings(this_object());
    foreach(object living in livings){
	if(living->GetRace() == "orc" && !interactive(living)) {
	    write("An orc bars your way!");
	    return 0;
	}
    }
    return 1;
}

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(15);
    SetShort("The Orc Fortress");
    SetLong("You find yourself in the entrance of the local "
      "orc stronghold. This place seems quite hostile to "
      "human sensibilities, as the construction is "
      "rough, inelegant, and the upkeep is squalid. This "
      "wooden fort also appears somewhat amateurishly "
      "put together, as if done by youngsters playing "
      "at being soldiers. There is an exit to the outside "
      "south of here, and another room is visible to the "
      "north.");
    SetSmell( ([ "default" : "The stench of garbage and animal waste hangs here."]) );
    SetExits( ([
	"south" : "/domains/town/room/valley",
      ]) );
    AddExit("north", "/domains/town/room/orc_temple", (: PreExit :));
    SetObviousExits("n,s");
    SetInventory(([
	"/domains/town/npc/orc" : 2,
	"/domains/town/npc/orc2" : 1,
	"/domains/town/npc/orc_boss" : 1,
      ]));
}
