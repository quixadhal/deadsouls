#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetAmbientLight(25);
    SetClimate("outdoors");
    SetShort("University Square");
    SetLong("You are at University Square, a cobblestone-paved crossroads with a large tree in the middle. To the east is the Virtual Campus administrative building entrance. There is a trash can here for keeping the area tidy. North is a path that seems to lead to a small town. The science building is south.");
    SetItems(([
        ({"building","administrative building"}) : "This large building houses the offices "+
        "of Virtual Campus staff, and is where most business is done. It also contains "+
        "a snack bar, a small store, and student lounge.",
        "clinic" : "This small clinic is where students in need of medical "+
        "attention go."
      ]) );
    //SetSkyDomain("town");
    SetExits( ([
        "north" : "/domains/campus/room/npath",
        "east" : "/domains/campus/room/foyer",
        "south" : "/domains/campus/room/science1.c",
      ]) );
    SetProperties(([
        "no attack" : 0,
      ]));
    SetInventory(([
        "/domains/campus/npc/tim" : 1,
        "/domains/campus/npc/wim" : 1,
        "/domains/campus/chamber/tree" : 1,
        "/domains/campus/obj/bench" : 3,
        "/domains/campus/obj/trashcan" : 1,
      ]));
    SetEnters( ([ 
      ]) );
    AddItem(new("/domains/campus/obj/lamp"));
}

void init(){
    ::init();
}
