#include <lib.h>
inherit LIB_ROOM;
inherit LIB_CLIMB;

static void create() {
    room::create();
    SetAmbientLight(25);
    SetClimate("outdoors");
    SetShort("University Square");
    SetLong("You are at University Square, a cobblestone-paved crossroads with a large tree in the middle. To the east is the Virtual Campus administrative building entrance. North is a path that seems to lead to a small town. The classroom building is south. The new Science Building is west.");
    SetItems(([
        ({"building","administrative building"}) : "This large building houses the offices "+
        "of Virtual Campus staff, and is where most business is done. It also contains "+
        "a snack bar, a small store, and student lounge.",
        "clinic" : "This small clinic is where students in need of medical "+
        "attention go."
      ]) );
    SetProperties(([
        "no attack" : 0,
      ]));
    SetExits( ([
        "south" : "/domains/campus/room/spath",
        "north" : "/domains/campus/room/npath",
        "east" : "/domains/campus/room/foyer",
        "west" : "/domains/campus/room/science1.c",
      ]) );
    SetInventory(([
        "/domains/campus/npc/tim" : 1,
        "/domains/campus/npc/wim" : 1,
        "/domains/campus/obj/tree" : 1,
        "/domains/campus/obj/bench" : 3,
      ]));
    SetEnters( ([ 
      ]) );

}
void init(){
    ::init();
}
