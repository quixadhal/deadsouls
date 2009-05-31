#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Probability Lab 1");
    SetLong("This is a clean, antiseptic lab with "
            "gleaming white walls. On the north wall are "
            "three doors: "
            "one %^RED%^red%^RESET%^, "
            "one %^GREEN%^green%^RESET%^, "
            "one %^BLUE%^blue%^RESET%^. "
            "\n"
            "%^GREEN%^There is a large sign on the wall you can read.%^RESET%^");
    SetInventory( ([
                "/domains/campus/obj/pedestal" : 1,
                "/domains/campus/npc/charly" : 1
                ]) );
    SetItems( ([
                ({ "wall","walls","north wall"}) : "Shiny, white laboratory walls.",
                "red room" : "There's no red room here.",
                "green room" : "There's no green room here.",
                "blue room" : "There's no blue room here.",
                ]) );
    SetObviousExits("north");
    SetExits( ([ 
                "north" : "/domains/campus/room/wiz_lab",
                ]) );
    AddItem( ({"sign","sign on the wall"}), "A sign you can read.",({"large"}) );

    SetEnters( ([
                "red room" : "/domains/campus/room/red_room",
                "green room" : "/domains/campus/room/green_room",
                "blue room" : "/domains/campus/room/blue_room"
                ]) );
    SetDoor("red room","/domains/campus/doors/red_door");
    SetDoor("green room","/domains/campus/doors/green_door");
    SetDoor("blue room","/domains/campus/doors/blue_door");
    SetNoClean(1);
}
void init(){
    ::init();
    SetRead(({"sign","sign on the wall"}), "Press the button on "
            "the pedestal to reset the experiment.", "Common");
}
