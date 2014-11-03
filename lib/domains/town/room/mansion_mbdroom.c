#include <lib.h>
inherit LIB_ROOM;

int open;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Mansion Master Bedroom");
    SetLong("You are in the master bedroom. The decor here is "
            "beautiful, ornate, and extravagant. It is obvious that "
            "the people who live here are rich and possess great "
            "taste. ");
    SetItems( ([
                "decor" : "Impressive, ornate, and beautiful.",
                ]) );
    SetInventory( ([
                "/domains/town/obj/bed" : 1,
                "/domains/town/obj/wardrobe" : 1,
                ]) );
    SetExits( ([
                "north" : "/domains/town/room/mansion_uhall3"
                ]) );
    open = 0;

    SetDoor("north", "/domains/town/doors/m7.c");

}
int OpenPassage(){
    string desc;
    if(open == 1) return 1;
    tell_room(environment(this_player()),"A secret passageway is revealed!");
    open = 1;
    desc = GetLong();
    desc += "\n%^MAGENTA%^There is a newly-opened secret passageway here. %^RESET%^";
    SetLong(desc);
    AddItem(({"secret passageway","passageway"}) , "A passageway you can enter.");
    SetEnters(([ 
                "passageway" : "/domains/town/room/mansion_room11",
                "secret passageway" : "/domains/town/room/mansion_room11"
                ]) );
    return 1;
}
void init(){
    ::init();
}
