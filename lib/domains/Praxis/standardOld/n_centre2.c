#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("night light", 1);
    set_property("no castle", 1);
    set_short( "The north end of Praxis");
    set_long(
      "Centre Path turns into nothing more than a small path "
      "through the North Forest north of here as you approach "
      "the northernmost part of Praxis.  South the path leads into "
      "the heart of this grand village.  A small hospital is west. "
      "Sun alley runs east.");
    set_exits( 
      (["north" : "/domains/Praxis/forest1",
	"south" : "/domains/Praxis/n_centre1",
	"east" : "/domains/Praxis/sun1",
	"west"	 : "/domains/Praxis/hospital"]) );
    set_items(
      (["path" : "Centre Path leads to Krasna Square south and out "
	"of town north.",
	"road" : "Boc La Road is the main east-west road.",
	"church" : "It is a small church run by the local clerics.",
	"forest" : "The North Forest.  It is not a very well-"
	"traveled place.",
	"village" : "The grand village of Praxis",
	"hospital" : "It is a primson white building jutting out "
	"from the landscape"]) );
}

