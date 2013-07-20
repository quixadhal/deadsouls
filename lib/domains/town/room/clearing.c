#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(40);
    SetNightLight(10);
    SetDayLight(30);
    SetShort("Forest Clearing");
    SetLong("This is a small clearing in a path through the forest. The path leads west into a small valley, and northeast toward a forest and a bridge.");
    SetItems( ([
        "clearing" : "An area with few trees.",
        "path" : "This forest path runs east "
        "and west. A small trail goes north of here.",
        ({"forest","wood","woods"}) : "This is a "
        "dark, creepy forest. You are standing on a "
        "path in a clearing, so it's a bit brighter here.",
        ({ "small trail","trail"}) : "A small trail leading into the depths "
        "of the forest to the north.",
        "valley" : "A valley is west of here.",
        "town" : "A town, far in the eastern distance."
      ]) );
    SetSkyDomain("town");
    SetExits( ([ 
        "northeast" : "/domains/town/room/forest_path1",
        "west" : "/domains/town/room/valley",
      ]) );
}
void init(){
    ::init();
}
