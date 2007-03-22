#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Forest Path");
    SetLong("You are in a forest outside the town. "
      "To the west the path continues toward a clearing. To the "
      "east it leads to a bridge over a river and returns to the village. "
      "Trees and bushes of various sorts border the path. ");
    SetItems( ([
	"forest" : "You are on a path in a dark, creepy forest. The "
	"vegetation is thick and feels somehow oppressive.",
	({"plants","bushes","vegetation","tree","trees","bush"}) : 
	"The plants, bushes, and trees grow thick and wild "
	"here, limiting visibility beyond the path.",
	"bridge" : "An old bridge is east of here.",
	"clearing" : "It looks like there is a clearing in the "
	"forest to the west.",
	"path" : "You are on a path in a forest."
      ]) );
    SetExits( ([
	"east" : "/domains/town/room/bridge",
	"west" : "/domains/town/room/clearing",
	"north" : "/domains/town/virtual/forest/24,1" ,
      ]) );
}
void init(){
    ::init();
}

int CanReceive(object ob){
    if(!interactive(ob) && starts_with(base_name(environment(ob)),"/domains/town/virtual") &&
      (!ob->GetRiders() || !sizeof(ob->GetRiders()))){
	write("You find yourself unable to leave the forest.");
	return 0;
    }
    return ::CanReceive();
}
