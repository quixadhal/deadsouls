#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Laboratory Wing");
    SetLong("This is a bright, shiny laboratory complex connecting to labs where probability experiments are performed. Probability experiments are currently running in the south and southwest labs. The east lab is available for general use. The main hallway of the science building is north.");
    SetItems( ([
                ({"lab","laboratory","wing","complex"}) : "You "
                "are in the lab complex. Laboratories can "
                "be accessed from here.",
                ({"stairs","downstairs"}) : "Go down to "
                "return to the Creators' Hall."
                ]) );
    SetExits( ([
                "south" : "/domains/campus/room/monty",
                "north" : "/domains/campus/room/science1",
                "east" : "/domains/campus/room/plab",
                "southwest" : "/domains/campus/room/plab2.c",
                ]) );

    SetDoor("north", "/domains/campus/doors/prob_door.c");

}
void init(){
    ::init();
}
