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
                "north" : "/domains/town/room/mansion_ext",
                "south" : "/domains/town/room/mansion_dhall2",
                "east" : "/domains/town/room/mansion_room9.c",
                ]) );
    SetDoor("north","/domains/town/doors/mansion");
}
void init(){
    ::init();
}
