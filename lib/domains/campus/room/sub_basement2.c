#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("sub basement");
    SetLong("This is a basement deep under the campus. An elevator is west, and a stairwell is south. The basement continues east.");
    SetItems(([
                ]));
    SetExits( ([
                "east" : "/domains/campus/room/sub_basement1",
                "west" : "/domains/campus/room/shaftb",
                "south" : "/domains/campus/room/stairwell2d.c",
                ]) );
    SetInventory(([
                ]));

    SetEnters( ([ 
                ]) );
}

void init(){
    ::init();
}
