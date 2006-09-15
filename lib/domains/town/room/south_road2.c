#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

int readSign() {
    this_player()->more("/domains/town/txt/warning_sign.txt");
    return 1;
}
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("South Saquivor Road");
    SetLong("This is a cobblestone road, leading north into town and south away from it. South of here appears to be some sort of University campus.");
    SetItems( ([
	({"road","cobblestone road"}) : "A "
	"cobblestone road, running north and south.",
	"sign":"A large sign on the road. To read it, 'read sign'.",
      ]) );
    AddTerrainType(T_ROAD);
    SetExits( ([
	"north" : "/domains/town/room/south_road1",
	"south" : "/domains/campus/room/npath2.c",
      ]) );
    SetInventory(([
      ]));
    SetRead("sign", (: readSign :) );

}
void init(){
    ::init();
}
