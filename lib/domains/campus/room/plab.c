#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Probability Lab Three");
    SetLong("This is a clean, antiseptic lab with "
            "gleaming white walls. On the east wall are "
            "three doors: "
            "one %^RED%^red%^RESET%^, "
            "one %^GREEN%^green%^RESET%^, "
            "one %^BLUE%^blue%^RESET%^. "
            "\n"
            "%^GREEN%^There is a large sign on the wall you can read.%^RESET%^");
    SetItems( ([
                ({ "wall","walls","north wall"}) : "Shiny, white laboratory walls.",
                "red room" : "There's no red room here.",
                "green room" : "There's no green room here.",
                "blue room" : "There's no blue room here.",
                ]) );
    SetObviousExits("west");
    SetInventory(([
                "/domains/campus/obj/pedestal3" : 1,
                ]));
    SetExits(([
                "west" : "/domains/campus/room/wiz_lab",
                ]));

    AddItem( ({"sign","sign on the wall"}), "A sign you can read.",({"large"}) );

    SetEnters( ([
                "red room" : "/domains/campus/room/red_room3",
                "green room" : "/domains/campus/room/green_room3",
                "blue room" : "/domains/campus/room/blue_room3"
                ]) );
    SetDoor("red room","/domains/campus/doors/red_door3");
    SetDoor("green room","/domains/campus/doors/green_door3");
    SetDoor("blue room","/domains/campus/doors/blue_door3");
    SetNoClean(1);
}
void init(){
    ::init();
    SetRead(({"sign","sign on the wall"}), "Press the button on "
            "the pedestal to reset the experiment.");
}
