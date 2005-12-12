#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Mansion Foyer");
    SetLong("This is the foyer of a grand, majestic mansion. Strangely, "
      "there is dust everywhere, but the place looks "
      "beautiful all the same. A hallway runs south "
      "from here."); 
    SetItems( ([
	"foyer" : "This is the interior entry area of the "
	"mansion, where the front door is.",
	({"hall","hallway"}) : "A wide corridor "
	"leading south to the rest of the mansion.",
	"dust" : "Tiny particulate material dispersed "
	"on visible surfaces."
      ]) );
    SetExits( ([
	"south" : "/domains/town/room/mansion_dhall2",
	"north" : "/domains/town/room/mansion_ext",
      ]) );
    SetDoor("north","/domains/town/doors/mansion");
    SetObviousExits("n,s");
}
