#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Church Basement");
    SetLong("This is the dark, musty basement of the village church. An elevator door is in the west wall.");
    SetItems(([
                ({"elevator","elevator door","door","wall","west wall"}) : "Set into the "+
                "west wall is an elevator. There is a button "+
                "next to it, presumably to call the elevator car.",
                ]) );
    SetInventory(([
                "/domains/town/obj/couch" : 1,
                "/domains/town/npc/leo" : ({60, 1})
                ]));
    SetExits( ([
                "west" : "/domains/town/room/elevator",
                ]) );
    //AddStuff( ({"/domains/town/npc/leo"}) );
}
void init(){
    ::init();
    if(!present("button",this_object())) AddItem(new("/domains/town/obj/basement_button"));
}
