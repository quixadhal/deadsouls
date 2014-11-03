#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetListen( "default", "The hustle of a town surrounds you.");
    SetShort( "East Boc La Road");
    SetLong(
            "Off to the  east you can see the intersection between Boc La Road "
            "and East Road. There are two buildings visible from here, Horace's "
            "Supply Store is to the north, and the building housing the "
            "adventurer's hall is to the south. People busily bustle "
            "back and forth, going about their business.");
    SetExits( 
            (["north" : "/domains/Praxis/supply",
             "south" : "/domains/Praxis/adv_main",
             "east" : "/domains/Praxis/e_boc_la3",
             "west" : "/domains/Praxis/e_boc_la1"]) );
    SetItems(
            (["shop" : "A small place where items can be bought and sold.",
             "road" : "A dirt path leading east and west through Praxis.",
             "area" : "Adventurers gather there to form parties and "
             "discuss their adventures.",
             "square" : "You cannot see it very well from here.",
             "people" : "The people walk by at a quicken pace, hardly bothering "
             "you with a glance.",
             "hall" : "The hall is where all the adventurer's of Praxis gather to "
             "chat."]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
}
