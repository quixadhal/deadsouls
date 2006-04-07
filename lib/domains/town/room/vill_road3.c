#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("East Village Road");
    SetLong("A long road going through the village. Town Hall is north. The main part of the town is west of here. The post office is south.");
    SetItems(([
	({ "office", "post office" }) : "Mail can be received and sent from the post office.",
	({ "building", "buildings" }) : "Structures designed for human occupancy.",
	({ "hall", "town hall" }) : "This is the modest building where official town business takes place.",
      ]));
    SetExits( ([
	"south" : "/domains/town/room/postoffice",
	"west" : "/domains/town/room/vill_road2",
	"north" : "/domains/town/room/thall",
	"east" : "/domains/town/room/vill_road4.c",
      ]) );
    SetEnters( ([
	"town hall" : "/domains/town/room/thall",
	"office" : "/domains/town/room/postoffice.c",
      ]) );
}
void init(){
    ::init();
}
