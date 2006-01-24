#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("West Village road");
    SetLong("This is a well-traveled road, leading east into town and west away from it. An old, humpbacked bridge is west of here.");
    SetItems(([
	({ "fort", "fortress", "fortress in the distance" }) : "It can't be seen well from here, but far north is what appears to be a large fortress built on a high plateau.",
	({ "road", "roads" }) : "This is a simple east-west road that goes east into town and west away from it. Another road, paved with cobblestones, intersects here to the north and leads high toward a fortress in the distance.",
	"cobblestone road" : "This is where a cobblestone road begins that is built on an steep incline and rises up as it leads north to a high plateau.",
      ]));
    SetExits( ([ 
	"west" : "/domains/town/room/bridge",
	"east" : "/domains/town/room/road1",
      ]) );

}
void init(){
    ::init();
}
