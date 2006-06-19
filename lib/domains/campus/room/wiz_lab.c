#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Laboratory Wing");
    SetLong("This is a bright, shiny laboratory complex connecting to labs where probability experiments are performed. Probability experiments are currently running in the north and west labs. The east lab is available for general use. The main hallway of the science building is south.");
    SetItems( ([
	({"lab","laboratory","wing","complex"}) : "You "
	"are in the lab complex. Laboratories can "
	"be accessed from here.",
	({"stairs","downstairs"}) : "Go down to "
	"return to the Creators' Hall."
      ]) );
    SetExits( ([
	"west" : "/domains/campus/room/plab2",
	"north" : "/domains/campus/room/monty",
	"east" : "/domains/campus/room/plab",
	"south" : "/domains/campus/room/science1.c",
      ]) );

    SetDoor("south", "/domains/campus/doors/prob_door.c");

}
void init(){
    ::init();
}
