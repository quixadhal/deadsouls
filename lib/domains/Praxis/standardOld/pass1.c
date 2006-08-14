#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("no castle", 1);
    set_short( "Eastern end of the Daroq Mountains Pass");
    set_long(
      "Down below this pass to the east is the Great Western Highway "
      "that crosses the lands of the east.  Curving west through "
      "the Daroq Mountain Range, the pass encounters the territory of "
      "goblins, balrogs, and tak'daroqs.  The highest point on the "
      "path is not too far west. ");
    set_items(
      (["pass" : "A gloomy pass through the Daroq Mountains.",
	"highway" : "The Great Western highway.  It leads into Praxis.",
	"lands" : "The fertile lands of the east.",
	"range" : "The Daroq Mountains, home of many vile creatures.",
	"mountains" : "The Daroq Mountains, home of many vile creatures.",
	"territory" : "It belongs to the evil creatures of the underground.",
	"point" : "From there, the pass heads down westward into "
	"the Qajip Desert."]) );
    set_exits( 
      (["northwest" : "/domains/Praxis/pass2",
	"east" : "/domains/Praxis/highway3"]) );
}

