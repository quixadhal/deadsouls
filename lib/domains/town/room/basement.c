#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(10);
    SetShort("The Church Basement");
    SetLong("This is the dark, musty basement of the village church. An elevator door is in the west wall.");
    SetItems(([
	({"elevator","elevator door","door","wall","west wall"}) : "Set into the "+
	"west wall is an elevator. There is a button "+
	"next to it, presumably to call the elevator car.",
      ]) );
    SetExits( ([
	"west" : "/domains/town/room/elevator",
      ]) );
    SetObviousExits("w");
    AddStuff( ({"/domains/town/npc/leo"}) );
}
void init(){
    if(!present("button",this_object())) AddItem(new("/domains/town/obj/basement_button"));
}
