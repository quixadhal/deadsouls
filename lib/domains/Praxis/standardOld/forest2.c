#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("no castle", 1);
    set_short("a path in the North Forest");
    set_long(
      "The small footpath is leading you further and further into the "
      "depths of North Forest. The forest becomes much denser, and the "
      "the trees indicate that this is the older section of the "
      "forest. The foliage is so dense, that little light can filter "
      "through. North leads further into the forest, and south takes you "
      "back towards the main village. East you can see another path heading off."); 
    set_exits( 
      (["north" : "/d/Praxis/forest3",
	"south" : "/d/Praxis/forest1",
	"east"	 : "/d/18/forest"]) );
    set_items(
      (["footpath" : "A small path through the huge North Forest.",
	"forest" : "The North Forest.  It is not a very well-traveled "
	"place.",
	({ "trees", "tree" }) : "The trees are huge and forebodding.",
	"foliage" : "The foliage is a very dark coloured green."
      ]) );
}



