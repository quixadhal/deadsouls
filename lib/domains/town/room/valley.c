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
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Orc Valley");
    SetLong("You are in a small valley nestled amongst mountains to the west "
      "and north.  To the south the forest is impassably thick. "
      "A tribe of orcs have chosen this valley is their home, and "
      "several hovels sit and various states of disrepair around you, "
      "along with several vast piles of refuse.  The orcs have built "
      "a wooden fortress to the north.  To the west stands a "
      "sheer, unclimbable cliff. High above on the cliff a giant column "
      "of fire pours into the sky.");
    SetExits( ([
	"east" : "/domains/town/room/clearing"
      ]) );
    AddExit("north", "/domains/town/room/orc_fortress", (: PreExit :));
    SetInventory(([
	"/domains/town/npc/orc" : 1,
	"/domains/town/npc/orc2" : 1,
      ]) );

}
void init(){
    ::init();
}

int CanReceive(object ob){
    if(answers_to("provost",ob)) return 0;
    return ::CanReceive();
}
