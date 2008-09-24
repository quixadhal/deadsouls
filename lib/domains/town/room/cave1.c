#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(5);
    SetShort("A Dark Cave");
    SetLong("This is a small cave at the base of a cliff. A stone door is set into the south wall.");
    SetInventory(([
        "/domains/town/weap/fellsword" : 1,
        "/domains/town/obj/scroll_resurrection" : 1,
      ]));
    SetExits(([
        "south" : "/domains/town/room/cave_entrance",
      ]));

    SetDoor("south", "/domains/town/doors/stone.c");

}
void init(){
    ::init();
}
