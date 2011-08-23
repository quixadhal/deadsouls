#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("no castle", 1);
    SetShort( "deep in the North Forest");
    SetLong(
      "The tiny footpath suddenly turns sharply to the northwest, " 
      "continuing further into the North Forest. The path is being "
      "overgrown by large amounts of undergrowth. Over in the corner you "
      "spy a mysterious dark cavern leading down into the depths "
      "of the earth. A path leads off to the northwest, and south heads "
      "back out of the forest.");
    SetItems(
      ([
        ({"path", "footpath"}) : "A small path running through North Forest.",
        "forest" : "North Forest is a very dark and evil place.",
        "undergrowth" : "The undergrowth slowly is assimilating the path.",
        "cavern" : "Its mysterious maw beckons you to enter."]) );
    SetSkyDomain("town");
    SetExits( ([ 
        "south" : "/domains/Praxis/forest2",
        "down" : "/domains/Praxis/kataan_join",
      ]) );
}

void init(){
    ::init();
}
