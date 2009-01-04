#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Downstairs Mansion Hallway South");
    SetLong("You are in the beautifully decorated downstairs "
            "hallway of the mansion. There are rooms to "
            "the east and west, and the hallway continues "
            "north. A large, "
            "circular marble staircase leads up to the "
            "second floor."); 
    SetItems( ([
                "hallway" : "An interior area providing access to "
                "other places in the house.",
                ({"mansion","house"}) : "You are in a beautiful mansion.",
                ({"stairs","staircase"}) : "A marvelously sculpted "
                "series of steps spiraling upward to the "
                "second floor. It is carved from marble."
                ]) );
    SetExits( ([ 
                "north" : "/domains/town/room/mansion_dhall2",
                "west" : "/domains/town/room/mansion_room8",
                "up" : "/domains/town/room/mansion_uhall3",
                ]) );

}
void init(){
    ::init();
}
