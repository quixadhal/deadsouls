#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Downstairs Mansion Hallway");
    SetLong("You are in the beautifully decorated downstairs "
            "hallway of the mansion. There are rooms to "
            "the east and west, and the hallway continues "
            "south. To the north is the entrance foyer.");
    SetItems( ([ 
                "hallway" : "An interior area providing access to "
                "other places in the house.",
                ({"mansion","house"}) : "You are in a beautiful mansion.",
                "foyer" : "The mansion's entry area is north of here."
                ]) );
    SetExits( ([
                "north" : "/domains/town/room/mansion_int",
                "south" : "/domains/town/room/mansion_dhall3",
                "west" : "/domains/town/room/mansion_room10",
                "east" : "/domains/town/room/mansion_room7.c",
                ]) );

}
void init(){
    ::init();
}
