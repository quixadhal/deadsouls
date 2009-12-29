#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Stairwell, bottom");
    SetLong("This is the bottom of the stairwell of the university Science building. The stairs go up from here. There is a crawlspace here under the stairs. The sub basement is north.");
    SetItems(([
                ({ "stairs", "stair" }) : "Steps which lead up.",
                "crawlspace" : "A crawlspace under the stairs.",
                ]));
    SetEnters( ([
                "crawlspace" : "/domains/campus/room/crawlspace2.c",
                ]) );
    SetClimate("indoors");
    SetExits( ([
                "up" : "/domains/campus/room/stairwell2c",
                "north" : "/domains/campus/room/sub_basement2.c",
                ]) );
}

void init(){
    ::init();
}
