#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("night light", 1);
    set_property("no castle", 1);
    set_smell_string( "default", "The faint smell of foliage wafts towards you.");
    set_short( "the east end of Praxis");
    set_long(
      "The dirt path brings you to the end of the village of Praxis. "
      "Boc La Road meets up with a smaller dirt path which leads "
      "out further east into the wilderness. The path is very "
      "narrow and looks unsafe. After travelling a couple of meters it "
      "is suddenly engulfed in a mass of green leaves and undergrowth. "
      "Up ahead, you can see the darker path of East road, leading to "
      "Praxis' cemetary. West takes you back along East Boc La road.");
    set_exits( 
      (["west" : "/d/Praxis/e_boc_la2",
	"east" : "/d/Praxis/wild1",
	"north" : "/d/Praxis/east_road1",
	"south" : "/d/milieu/room/stair"]) );
    set_items(
      (["road" : "It leads west toward the village center.",
	"village" : "Praxis is a small adventuring community "
	"of the Nightmare reality.",
	"wilderness" : "Stories tell of bandits and rogues out "
	"in the wilderness.",
	"path" : "The path leads in many directions from here, towards the "
	"Praxis cemetary, the wilderness, and back towards Monument Square.",
	"cemetary": "The Praxis cemetary is off to the north on East Road." ]) );
}

