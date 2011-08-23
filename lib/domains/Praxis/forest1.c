#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("no castle", 1);
    SetShort( "the North Forest");
    SetLong(
      "You are in a huge forest just north of the village of Praxis.  "
      "The forest extends in all directions, with a small path "
      "leading deeper into the forest north and into one of the main "
      "village roads south.");
    SetExits( 
      (["north" : "/domains/Praxis/forest2",
        "south"	 : "/domains/Praxis/n_centre2"]) );
    SetItems(
      (["path" : "A small path throught the huge North Forest.",
        "forest" : "The North Forest. It is not a very well traveled place.",
        "village" : "The grand village of Praxis"]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
}
