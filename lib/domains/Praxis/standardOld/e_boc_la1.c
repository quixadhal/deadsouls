#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("night light", 1);
    set_property("no castle", 1);
    set_short("East Boc La south of the monastary");
    set_long(
      "Boc La Road on the eastern side of Praxis is a large dirt road "
      "leading towards the center of town, Monument Square. Many "
      "footprints have pounded the road into a flat surface, easy to "
      "travel. Rusted old lamp posts stand on either side of the road, "
      "to light Praxis by night. A small path to the north leads into the "
      "village monastary. East Boc La road continues east from here.");
    set_exits( 
      (["north" : "/d/Praxis/monastery",
	"east" : "/d/Praxis/e_boc_la2",
	"west" : "/d/Praxis/square"]) );
    set_items(
      (["road" : "Boc La Road is the large path that runs east-west "
	"through Praxis.", 
	"monastery" : "The monks of Nightmare study and "
	"worship there.", 
	"square" : "Monument Square is the center point of "
	"all of Praxis. Many adventurers enjoying hanging out there, "
	"chatting with fellow citizens about life in Praxis."]) );
}

