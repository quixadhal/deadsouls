#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("sub basement");
    SetLong("This is a basement deep under the campus. The basement continues west from here. There is a doorway to the east.");
    SetExits( ([
                "east" : "/domains/campus/room/maintenance",
                "west" : "/domains/campus/room/sub_basement2.c",
                ]) );
    SetInventory(([
                ]));
    SetDoor("east", "/domains/campus/doors/plain_door2");
    SetEnters( ([ 
                ]) );
}

void init(){
    ::init();
}
