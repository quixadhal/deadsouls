#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("a wide path");
    SetLong("This is a paved path traveling north from University "
      "Square. It is lined by thick bushes on each "
      "side. The Virtual Campus is south of here. To "
      "the north you see what appears to be a town.");
    SetItems( ([
        ({ "bush","bushes","side","sides"}) : "Large, thick "
        "bushes crowd the sides of the path.",
        "path" : "A wide, cobblestone path running north "
        "and south.",
        "town" : "You can't quite see much of it from here."
      ]) );
    SetSkyDomain("town");

    SetExits( ([ "south" : "/domains/campus/room/usquare",
        "north" : "/domains/campus/room/npath2",
      ]) );
    AddItem(new("/domains/campus/obj/lamp"));
}
void init(){
    ::init();
}
