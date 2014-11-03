#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetListen("default", "Raucus noise is coming from "
            "inside the pub west.");
    SetShort( "Centre Path outside Lars' Pub");
    SetLong(
            "Centre Path leads through the heart of the business district "
            "of Praxis, and down towards the famous Port of Praxis. "
            "Rowdy customers can be seen heading west towards Lar's Pub. "
            "The building is very old and worn. The abuse of many drunken "
            "adventurer's has began to take its toll on the establishment. "
           );
    SetExits( 
            (["north" : "/domains/Praxis/square",
             "south" : "/domains/Praxis/s_centre2",
             "west" : "/domains/Praxis/pub"]) );
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetItems(
            (["path" : "Centre Path leads to Monument Square south and out "
             "of town north.",
             "road" : "Boc La Road is the main east-west road.",
             "pub" : "Lars Pub, now mostly obsolete, but it is the original.",
             "square" : "The central square of Praxis."]) );
    SetSkyDomain("town");
    SetProperty("no castle", 1);
}

void init(){
    ::init();
}
