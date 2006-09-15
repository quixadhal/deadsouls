#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

int readSign() {
    this_player()->more("/domains/town/txt/hints_sign.txt");
    return 1;
}

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Village Road Intersection");
    SetLong("You are in the main intersection of the village, "
      "where Saquivor road extends north and south, and a "
      "narrow track intersects the road from the west. The track "
      "is somewhat better kept as it continues eastward as a road.\n"+
      "%^GREEN%^There is a sign here you can read.%^RESET%^");

    SetItems( ([
	"sign":"A large sign on the road. To read it, 'read sign'.",
      ]) );
    AddTerrainType(T_ROAD);
    SetNoModify(0);
    SetInventory(([
	"/domains/town/obj/clocktower" : 1,
      ]));
    SetEnters( ([
      ]) );
    SetRead("sign", (: readSign :) );
    SetExits( ([
	"north" : "/domains/town/room/road",
	"east" : "/domains/town/room/vill_road2",
	"west" : "/domains/town/room/road1",
	"south" : "/domains/town/room/south_road1",
      ]) );
}

void init(){
    ::init();
}
