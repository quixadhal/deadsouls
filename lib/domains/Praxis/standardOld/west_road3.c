#include <std.h>

inherit ROOM;

void init() {
    ::init();
    add_action("enter", "enter");
}

void create() {
    ::create();
    set_property("light", 2);
    set_short( "The north end of West Road");
    set_long(
      "West Road comes to an end in the forest to the north.  "
      "There is a huge, ancient tree which provides cover to the "
      "entire end area of the road.  A hole opens into a hollow inside.  "
      "Boc La Road is far to the south.");
    set_items(
      (["tree" : "It must be older than Praxis itself.",
	"hole" : "You might even be able to enter the tree.",
	"road" : "This end of the road is shaded by the huge "
	"tree of mysterious origins.",
	"foo" : "You're daft.",
	"bar" : "Had to check that too, eh?"]) );
    set_exits( 
      (["south" : "/domains/Praxis/west_road2"]) );
}

int enter(string str) {
    if(!str) {
	notify_fail("Enter what?\n");
	return 0;
    }
    if(str != "hole" && str != "tree") {
	notify_fail("You can't do that here.\n");
	return 0;
    }
    this_player()->move_player("/domains/Praxis/mage_join", "into the tree");
    return 1;
}

