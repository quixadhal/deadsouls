#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no castle", 1);
    set_short( "Monastary stairwell");
    set_long(
      "A spiraling flight of stairs leads up to the monastery "
      "attic and down to the cellar.  The prayer area is west.");
    set_exits( 
      (["west" : "/domains/Praxis/monastery",
	"up" : "/domains/Praxis/monk_join",
	"down" : "/domains/Praxis/immortal_hall"]) );
    set_items(
      (["stairs" : "They spiral up and down in this old monastery."]) );
    set_property("no castle", 1);
}


