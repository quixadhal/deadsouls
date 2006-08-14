#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_listen_string("default", "Raucus noise is coming from "
      "inside the pub west.");
    set_short( "Centre Path outside Lars' Pub");
    set_long(
      "Going south from Krasna Square, Centre Path leads through "
      "the business district of Praxis towards the Port of Praxis.  "
      "Customers are going in and out of Lars' Pub west.");
    set_exits( 
      (["north" : "/domains/Praxis/square",
	"south" : "/domains/Praxis/s_centre2",
	"west" : "/domains/Praxis/pub"]) );
    set_property("light", 3);
    set_property("night light", 1);
    set_items(
      (["path" : "Centre Path leads to Monument Square south and out "
	"of town north.",
	"road" : "Boc La Road is the main east-west road.",
	"pub" : "Lars Pub, now mostly obsolete, but it is the original.",
	"square" : "The central square of Praxis."]) );
    set_property("no castle", 1);
}

