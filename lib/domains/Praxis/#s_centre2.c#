#include <std.h>

inherit ROOM;

void create() {
  ::create();
    set_property("light", 3);
    set_property("night light", 2);
    set_property("no castle", 1);
    set_short( "Centre Path just south of Krasna Square");
    set_long(
      "A small alley crosses Centre Path here just south of Krasna Square.  "
      "As it winds on south, Centre Path travels through the business "
      "center of South Praxis up to the Port of Praxis.");
    set_exits( 
	      (["north" : "/domains/Praxis/s_centre1",
		"south" : "/domains/Praxis/s_centre3",
		"east" : "/domains/Praxis/yard",
		"west" : "/domains/Praxis/alley1"]) );
    set_items(
      ([({ "path", "road", "centre path" }) : "Centre Path leads to "
	  "Monument Square north and to the port south.",
	"alley" : "It opens up to a yard of houses east, and a post "
	  "and bank west."]) );
}

