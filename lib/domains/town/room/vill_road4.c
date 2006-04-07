#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Village Path");
    SetLong("As it travels from west to east, Village Road becomes less of a road here and more of a dirt path. The shore of the eastern sea is almost visible from here. The village schoolhouse is north, and the shore is east.");
    SetItems(([
	({ "building", "schoolhouse", "school", "village school", "village schoolhouse" }) : "A small but well-kept wooden building where townsfolk and their children go to acquire knowledge.",
	({ "building", "buildings" }) : "Structures designed for human occupancy.",
	({ "shore", "village shore" }) : "The shore of the eastern sea is east of here.",
      ]));
    SetExits( ([
	"west" : "/domains/town/room/vill_road3",
	"east" : "/domains/town/room/shore",
	"north" : "/domains/town/room/school.c",
      ]) );
    SetEnters( ([ 
      ]) );

}
void init(){
    ::init();
}
