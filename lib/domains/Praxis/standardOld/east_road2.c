#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("night light", 1);
    set_property("no castle", 1);
    set_listen_string( "default", "The quiet chirp of grasshoppers hangs "
      "in the air." );
    set_short( "the crossing of East Road and Sun Alley");
    set_long(
      "Sun Alley, named for its' beautiful view of the sunsets, shoots "
      "off west from East Road. East Road continues, heading north and "
      "south upon the eastern border of town. To the east, you can see "
      "the Praxis cemetary, shaded by large trees. The air is very still "
      "here, and a feeling of foreboding hangs in the air.");
    set_items(
      (["alley" : "A bright and cheery alley which marks the northern "
	"boarder of Praxis.",
	"road" : "East Road is a curvy road running north and south "
	"on the east side of Praxis.",
	"cemetary" : "The residents of Praxis get buried there. It "
	"looks spooky.",
	"trees" : "The trees are large drooping willow trees.",
	"forest" : "A great forest the marks the northern end of Praxis."])
    );
    set_exits( 
      (["north" : "/d/Praxis/east_road3",
	"south" : "/d/Praxis/east_road1",
	"east" : "/d/Praxis/cemetary/grave_yard",
	"west"	 : "/d/Praxis/sun2"]) );
}



