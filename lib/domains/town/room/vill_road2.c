#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("East Village Road");
    SetLong("You are on a long road. The "
      "general store is north. The road stretches "
      "east and west through the town. The Adventurers "
      "Guild is south.");
    SetItems( ([
	({"adventurers guild","guild"}) : "This small "
	"building on the southern side of the road is where "
	"adventurers, great and small, come to record "
	"their exploits and seek advancement.",
	({"shop","store","general store"}) : "This is the "
	"general store, where almost anything can be "
	"bought or sold.",
	({"road","long road"}) : "An east-west cobblestone "
	"road through town.",
      ]) );
    SetExits( ([
	"north" : "/domains/town/room/shop",
	"south" : "/domains/town/room/adv_guild",
	"east" : "/domains/town/room/vill_road3",
	"west" : "/domains/town/room/vill_road1",
      ]) );
    SetEnters( ([
	"guild" : "/domains/town/room/adv_guild",
	"adventurers guild" : "/domains/town/room/adv_guild",
	"shop" : "/domains/town/room/shop",
	"store" : "/domains/town/room/shop",
	"general store" : "/domains/town/room/shop",
      ]) );
}
void init(){
    ::init();
}
