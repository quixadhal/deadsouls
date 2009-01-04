#include <lib.h>

inherit LIB_FISHING;

void create() {
    ::create();
    SetProperty("no castle", 1);
    SetProperty("light", 3);
    SetProperty("night light", 2);
    SetMaxFishing(4);
    SetChance(10);
    SetShort( "Pier 1 of the Port of Praxis");
    SetLong(
            "Welcome to the Port of Praxis!\n"
            "The Pier 1 port is virtually bustling with activity. Ships from "
            "all over the reality dock here to deliver their goods to Praxis "
            "and its surrounding regions. The pier is made out of solid wood "
            "which has become weathered due to the rain and salty air. Torches "
            "line each side of the dockss, lighting the way at night. All along "
            "the pier you can see people fishing, talking and enjoying the weather.");
    SetSmell("default", "Rotting fish offend your nostrils.");
    SetSmell("fish", "They smell as if they have been out for days.");
    SetItems(
            (["port" : "Ships from all over Nightmare dock here to "
             "do business in Praxis.",
             "pier" : "Pier 1 of the port.",
             "goods" : "Merchandise from exotic places.",
             "torch" : "It lights up the pier at night.",
             "torches" : "They light up the pier at night."]) );
    SetExits( 
            (["north" : "/domains/Praxis/s_centre4" 	
             ]) );
}

void reset() {
    int i;

    ::reset();
    if(!present("stick"))
        new("/domains/Praxis/obj/misc/fishing_pole")->move(this_object());
    if(present("match")) return;
    i = 4;
    while(i--) new("/domains/Praxis/obj/misc/match")->move(this_object());
}
void init(){
    ::init();
}
