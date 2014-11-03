#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetCoordinates("3999,4000,-2");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("a tiny crawlspace");
    SetLong("You are in a cramped little space beneath the stairs in the "+
            "basement. It is dark, tight, and dirty in here.");
    SetItems(([
                ({"crawlspace","space"}) : "You are in the space beneath stairs.",
                "basement" : "The basement is just outside the crawlspace.",
                "stairs" : "You're under them."]));
    SetInventory(([
                "/domains/campus/obj/rayovac" : 1,
                ]));
    AddExit("out" , "/domains/campus/room/basement");
    SetObviousExits("out");
    SetProperty("no attack", 1);
    SetMoney( ([ "dollars" : random(30)+2, ]) );
}
void reset(){
    ::reset();
}
