#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetProperty("no castle", 1);
    SetSmell("default", "You smell the aroma of incense.");
    SetShort( "Mora's hut");
    SetLong( 
            "A mystical aura hangs about this otherwise modest hut "
            "decorated with strange artifacts from distant worlds.  "
            "This must be the hut of the famous medium Mora.");
    SetExits( 
            (["west" : "/domains/Praxis/west_road2"]) );
    SetItems(
            (["artifacts" : "You have no idea what they are or what they mean.",
             "hut" : "It is very odd."]) );
}

void reset() {
    ::reset();
    if(present("mora")) return;
    new("/domains/Praxis/obj/mon/mora")->move(this_object());
}
void init(){
    ::init();
}
