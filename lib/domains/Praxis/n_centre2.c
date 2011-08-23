#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort( "The north end of Praxis");
    SetLong(
      "Centre Path turns into nothing more than a small path "
      "through the North Forest north of here as you approach "
      "the northernmost part of Praxis.  South the path leads into "
      "the heart of this grand village.  A small hospital is west. "
      "Sun alley runs east.");
    SetExits( 
      (["north" : "/domains/Praxis/forest1",
        "south" : "/domains/Praxis/n_centre1",
        "east" : "/domains/Praxis/sun1",
        "west"	 : "/domains/Praxis/hospital"]) );
    SetItems(
      (["path" : "Centre Path leads to Krasna Square south and out "
        "of town north.",
        "road" : "Boc La Road is the main east-west road.",
        "church" : "It is a small church run by the local clerics.",
        "forest" : "The North Forest.  It is not a very well-"
        "traveled place.",
        "village" : "The grand village of Praxis",
        "hospital" : "It is a primson white building jutting out "
        "from the landscape"]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
}
