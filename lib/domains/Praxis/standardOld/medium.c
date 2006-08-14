#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no castle", 1);
    set_smell_string("default", "You smell the aroma of incense.");
    set_short( "Mora's hut");
    set_long( 
      "A mystical aura hangs about this otherwise modest hut "
      "decorated with strange artifacts from distant worlds.  "
      "This must be the hut of the famous medium Mora.");
    set_exits( 
      (["west" : "/domains/Praxis/west_road2"]) );
    set_items(
      (["artifacts" : "You have no idea what they are or what they mean.",
	"hut" : "It is very odd."]) );
}

void reset() {
    ::reset();
    if(present("mora")) return;
    new("/domains/Praxis/obj/mon/mora")->move(this_object());
}

