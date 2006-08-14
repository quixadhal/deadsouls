#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("night light", 1);
    set_property("no castle", 1);
    set_short( "Outside the financial district");
    set_long(
      "Leading out east into the rural lands outside of Praxis, this unnamed "
      "dirt road sits almost entirely untravelled.  The business district "
      "of Praxis is west.");
    set_exits( 
      ([ "west" : "/domains/Praxis/s_centre3",
	"south" : "/domains/Praxis/realty.c"
      ]) );
    set_items( 
      (["road" : "It is temporarily blocked going east."]) );
}

