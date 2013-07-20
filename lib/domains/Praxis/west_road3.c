#include <lib.h>

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("enter", "enter");
}

void create() {
    ::create();
    SetProperty("light", 2);
    SetShort( "the north end of West Road");
    SetLong(
      "West Road comes to an abrupt end as it is engulfed by the forest "
      "to the north. A huge ancient willow tree provides shelter to "
      "this entire section of the road. At its massive roots you can "
      "see a whole leading down into the depths of the earth. Boc La Road "
      "lies far south of here.");
    SetItems(
      (["tree" : "It must be older than Praxis itself.",
        "hole" : "You might even be able to enter the tree.",
        "road" : "This end of the road is shaded by the huge "
        "tree of mysterious origins.",
        "foo" : "You're daft.",
        "bar" : "Had to check that too, eh?"]) );
    SetSkyDomain("town");
    SetExits( 
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
    this_player()->eventMoveLiving("/domains/Praxis/mage_join", "into the tree");
    return 1;
}
