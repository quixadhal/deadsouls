#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("a tiny crawlspace");
    SetLong("You are in a cramped little space beneath the stairs in the "+
            "basement. It is dark, tight, and dirty in here.");
    SetItems(([
                ({"crawlspace","space"}) : "You are in the space beneath stairs.",
                "basement" : "The basement is just outside the crawlspace.",
                "stairs" : "You're under them."]));
    SetExits( ([ 
                "out" : "/domains/campus/room/stairwell2d"
                ]) );
    SetInventory(([
                ]));
}
void reset(){
    ::reset();
}
