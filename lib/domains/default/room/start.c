#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the start room");
    SetLong("The default start room. To enter "+
      "a sample set of rooms, go down. To visit Frontiers MUD, "+
      "go north.");
    SetExits( ([
	"down" : "/domains/Ylsrim/room/bazaar",
	"north" : "/domains/default/room/telnet_room"
      ]) );
    SetObviousExits("n,d");


}
