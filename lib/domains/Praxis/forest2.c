#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetProperty("no castle", 1);
    SetShort("a path in the North Forest");
    SetLong(
      "The small footpath is leading you further and further into the "
      "depths of North Forest. The forest becomes much denser, and the "
      "the trees indicate that this is the older section of the "
      "forest. The foliage is so dense, that little light can filter "
      "through. North leads further into the forest, and south takes you "
      "back towards the main village. East you can see another path heading off."); 
    SetItems(
      (["footpath" : "A small path through the huge North Forest.",
        "forest" : "The North Forest.  It is not a very well-traveled "
        "place.",
        ({ "trees", "tree" }) : "The trees are huge and forebodding.",
        "foliage" : "The foliage is a very dark coloured green."
      ]) );
    SetSkyDomain("town");
    SetExits( ([ 
        "south" : "/domains/Praxis/forest1",
        "north" : "/domains/Praxis/forest3",
      ]) );
}
void init(){
    ::init();
}
