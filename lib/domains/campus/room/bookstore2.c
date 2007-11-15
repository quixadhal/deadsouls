#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetAmbientLight(30);
    SetClimate("indoors");
    SetShort("a generic room");
    SetLong("This is an utterly plain, blank room.");
    SetInventory(([
        "/domains/default/obj/handbook" : 5,
        "/domains/campus/obj/bag" : 3,
        "/domains/campus/obj/pack" : 30,
        "/domains/default/obj/manual" : 5,
        "/domains/default/obj/guide" : 5,
      ]));
}
void init(){
    ::init();
}
