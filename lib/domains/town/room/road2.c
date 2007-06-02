#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

string LongDesc(){
    string desc = "This is a well-traveled road, leading east into town and west away from it. An old, humpbacked bridge is west of here.";
    if(this_object()->GetExit("north")) 
	desc += " A road north leads up to a plateau where a fortress stands in the  distance.";
    return desc;
}

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("West Village road");
    SetLong((: LongDesc :));
    SetItems(([
	({ "fort", "fortress", "fortress in the distance" }) : "It can't be seen well from here, but far north is what appears to be a large fortress built on a high plateau.",
	({ "road", "roads" }) : "This is a simple east-west road that goes east into town and west away from it. Another road, paved with cobblestones, intersects here to the north and leads high toward a fortress in the distance.",
	"cobblestone road" : "This is where a cobblestone road begins that is built on an steep incline and rises up as it leads north to a high plateau.",
      ]));
    AddTerrainType(T_ROAD);
    SetExits( ([ 
	"east" : "/domains/town/room/road1",
	"west" : "/domains/town/room/bridge",
      ]) );

}

void init(){
    ::init();
    if(mud_name() == "Dead Souls Omega" && !GetExit("north")){
	AddExit("north", "/domains/fort/room/f_road4");
    }
}
