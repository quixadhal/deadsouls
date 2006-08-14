#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("night light", 1);
    set_property("no castle", 1);
    set_listen_string( "default", "The hustle of a town surrounds you.");
    set_short( "East Boc La Road");
    set_long(
      "Off to the  east you can see the intersection between Boc La Road "
      "and East Road. There are two buildings visible from here, Horace's "
      "Supply Store is to the north, and the building housing the "
      "adventurer's hall is to the south. People busily bustle "
      "back and forth, going about their business.");
    set_exits( 
      (["north" : "/d/Praxis/supply",
	"south" : "/d/Praxis/adv_main",
	"east" : "/d/Praxis/e_boc_la3",
	"west" : "/d/Praxis/e_boc_la1"]) );
    set_items(
      (["shop" : "A small place where items can be bought and sold.",
	"road" : "A dirt path leading east and west through Praxis.",
	"area" : "Adventurers gather there to form parties and "
	"discuss their adventures.",
	"square" : "You cannot see it very well from here.",
	"people" : "The people walk by at a quicken pace, hardly bothering "
	"you with a glance.",
	"hall" : "The hall is where all the adventurer's of Praxis gather to "
	"chat."]) );
}

