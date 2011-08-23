#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("a wide path");
    SetLong("This is a paved path traveling north and south. It is lined by thick bushes on each side. A town lies north of here. To the south is what appears to be a college campus.");
    SetItems( ([
        ({ "bush","bushes","side","sides"}) : "Large, thick "
        "bushes crowd the sides of the path.",
        "path" : "A wide, cobblestone path running north "
        "and south.",
        ({ "sign","post","signpost"}) : "A post has been "
        "driven into the ground here, and a sign is attached "
        "to it.",
        "town" : "You can't quite see much of it from here.",
        "campus" : "You can't quite see much of it from here."
      ]) );
    SetSkyDomain("town");
    SetInventory(([
      ]));
    SetRead( "sign", "Beware! You are now leaving the safety of the Virtual Campus.");
    SetExits( ([ "south" : "/domains/campus/room/npath",
        "north" : "/domains/campus/room/south_road2",
      ]) );
    AddItem(new("/domains/campus/obj/lamp"));
}
void init(){
    ::init();
}
