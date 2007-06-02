#include <lib.h>
inherit LIB_ROOM;

int PreExit(){
    object *livings;
    string *allowed_races = ({ "orc", "half-orc", "bear" });
    string duderace = this_player()->GetRace();
    livings = get_livings(this_object());
    foreach(object living in livings){
	if(living->GetRace() == "orc" && !interactive(living) &&
	  member_array(duderace, allowed_races) == -1){
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
    SetLong("You are in a small valley below the sheer cliffs to the west. A band of orcs appears to have made this place their home, and you can see their ramshackle fortress toward the north. A well-traveled path leads east into a forest.");
    //A narrow path snakes its way southwest into the woods.");
    SetExits( ([
	"east" : "/domains/town/room/clearing",
	//"southwest" : "/domains/town/room/narrow_path.c",
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
