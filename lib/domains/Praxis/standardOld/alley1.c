#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("night light", 1);
    set_property("no castle", 1);
    set_short("A narrow alley");
    set_smell_string( "default", "The faint smell of rubbish assaults you.");
    set_long(
      "You find yourself on a small dirt path which leads to and from "
      "Centre Path.  The path is a dirt road, cluttered occasionally "
      "with garbage from some of the less considerate citizens of Praxis. "
      "The local Post office is to the south, and to the north you can see "
      "a large bulding which serves as the bank of Praxis.");
    set_exits( 
      (["north" : "/d/Praxis/bank",
	"south" : "/d/Praxis/post",
	"east" : "/d/Praxis/s_centre2",
	"west" : "/d/Praxis/alley2"]) );
    set_items( 
      (["path" : "The path is a small dirt road leading from Centre Path.",
	"bank" :"The bank of Praxis is a very well known establishment.",
	({"office", "post", "post office"}) : "You can send and receive "
	"mail to other people here or on other muds there.",
	"alley" : "The alley is a very small dirt path."]) );
}

