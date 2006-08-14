#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("no castle", 1);
    set_short( "deep in the North Forest");
    set_long(
      "The tiny footpath suddenly turns sharply to the northwest, " 
      "continuing further into the North Forest. The path is being "
      "overgrown by large amounts of undergrowth. Over in the corner you "
      "spy a mysterious dark cavern leading down into the depths "
      "of the earth. A path leads off to the northwest, and south heads "
      "back out of the forest.");
    set_exits( 
      (["south" : "d/Praxis/forest2",
	"northwest" : "/d/north_forest/path1",
	"down"	 : "/d/Praxis/kataan_join"]) );
    set_items(
      ([
	({"path", "footpath"}) : "A small path running through North Forest.",
	"forest" : "North Forest is a very dark and evil place.",
	"undergrowth" : "The undergrowth slowly is assimilating the path.",
	"cavern" : "Its mysterious maw beckons you to enter."]) );
}
