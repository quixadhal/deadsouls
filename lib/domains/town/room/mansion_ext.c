#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(40);
    SetShort("Mansion Exterior");
    SetNightLong("You are standing outside a large, beautiful mansion to the south, illuminated by lamplight. It towers over you and overhead you can see one of the second floor windows is open. The grass seems a bit wild around here, and the lawn continues west into the darkness. The mansion gate is north.");
    SetDayLong("You are standing outside a large, beautiful mansion to the south. It towers over you and overhead you can see one of the second floor windows is open. The grass seems a bit wild around here, and the lawn continues west into an overgrown garden. The mansion gate is north.");
    SetLong("You are standing outside a large, beautiful mansion "
      "that stands south of you. It towers over you and "
      "overhead you can see one of the second floor "
      "windows is open. The grass seems a bit wild "
      "around here, and the lawn continues west into "
      "an overgrown garden. The mansion gate is north.");
    SetItems( ([
        ({"house","mansion"}) : "A stately pleasure dome.",
        ({"window","second floor window"}) : "This is "
        "an open window, high above you. If you had a ladder, you "
        "might be able to climb up enough to enter.",
        ({"grass","lawn","garden"}) : "Rather unkempt "
        "greenery. Looks like the gardener's been on "
        "vacation.",
        ({"gate","mansion gate"}) : "The entry point into "
        "the estate on which you now stand."
      ]) );
    SetSkyDomain("town");
    SetExits( ([
        "north" : "/domains/town/room/gate",
        "west" : "/domains/town/room/garden",
        "south" : "/domains/town/room/mansion_int",
      ]) );
    SetDoor("south","/domains/town/doors/mansion");
    AddItem(new("/domains/town/obj/lamp"));
}
void init(){
    ::init();
}
