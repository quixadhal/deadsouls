#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Mansion Garden");
    SetLong("This is what is left of what was once a beautiful, "
      "if simple, garden. It is now badly overgrown with "
      "weeds, and it is clear that foraging animals long "
      "ago ate the delicate flowers and vegetables that "
      "once grew here. The gardener's shack is north.");
    SetItems( ([
	"garden" : "A mess of weeds and dirt.",
	({"flowers","vegetables","flowers and vegetables"}) :
	"If they were once here, they certainly no longer are.",
	({"wooden" ,"shack","gardener's shack"}) : "A beat-up old "
	"wooden shack is to the north."
      ]) );
    SetExits( ([
	"east" : "/domains/town/room/mansion_ext",
	"north" : "/domains/town/room/shack"
      ]));
    SetEnters( ([
	"shack" : "/domains/town/room/shack",
	"gardener's shack" : "/domains/town/room/shack"
      ]) );
    SetObviousExits("n,e");
}
