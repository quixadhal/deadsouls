#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetNightLight(10);
    SetDayLight(30);
    SetShort("Forest Path");
    SetLong("You are in a forest outside the town. To the southwest the path continues toward a clearing. To the east it leads to a bridge over a river toward a town. Trees and bushes of various kinds border the path making it impossible to go south but to the north the vegetation is less dense.");
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
    SetSkyDomain("town");
    SetExits( ([
        "north" : "/domains/town/virtual/forest/-4,1",
        "east" : "/domains/town/room/bridge",
        "southwest" : "/domains/town/room/clearing.c",
      ]) );
}
void init(){
    ::init();
}

int CanReceive(object ob){
    object env = environment(ob);
    if(!env || base_name(env) != "/domains/town/virtual/forest/24,1"){
        return ::CanReceive();
    }
    if(living(ob) && !interactive(ob) &&
      (!ob->GetRiders() || !sizeof(ob->GetRiders()))){
        ob->eventPrint("You find yourself unable to leave the forest.");
        return 0;
    }
    return ::CanReceive();
}
