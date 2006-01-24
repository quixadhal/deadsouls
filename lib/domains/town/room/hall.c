#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("copy of vill_road3.c");
    SetLong("A long road going through the village." +
      "Town Hall is north. The main part of the town "+
      "is west of here. The post office is south.");
    SetItems(([
	({ "town hall", "hall" }) : "A place to conduct official business.",
	({ "post office", "office", "post" }) : "A place to send and receive mail.",
      ]));
    SetExits(([
      ]));

    SetEnters( ([
	"office" : "/domains/town/room/postoffice.c",
      ]) );
}
void init(){
    ::init();
}
