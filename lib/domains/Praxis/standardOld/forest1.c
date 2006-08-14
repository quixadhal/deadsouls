#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("no castle", 1);
    set_short( "the North Forest");
    set_long(
      "You are in a huge forest just north of the village of Praxis.  "
      "The forest extends in all directions, with a small path "
      "leading deeper into the forest north and into one of the main "
      "village roads south.");
    set_exits( 
      (["north" : "/d/Praxis/forest2",
	"south"	 : "/d/Praxis/n_centre2"]) );
    set_items(
      (["path" : "A small path throught the huge North Forest.",
	"forest" : "The North Forest. It is not a very well traveled place.",
	"village" : "The grand village of Praxis"]) );
}

