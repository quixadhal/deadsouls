#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 4);
    set_property("no castle", 1);
    set_short( "Mountain pass");
    set_long(
      "You are at the highest point of a dark pass through the "
      "Daroq Mountains.  Down below to the west you can see the "
      "vast and barren Qajip Desert.  In the valley east is a "
      "tremendous forest.  A small cave opens up into the mountain.");
    set_items(
      (["point" : "From here it looks like you can see the entire world.",
	"pass" : "It winds around from the west to the southeast.",
	"mountains" : "A huge mountain range separating the fertile "
	"lands of the east from the desert.",
	"valley" : "A huge forested valley.",
	"desert" : "The Qajip Desert where many travellers have "
	"lost their way and not been found.",
	"forest" : "It is dark and gloomy near the mountains, but "
	"it looks friendlier east.",
	"cave" : "A very dark and ominous cave.",
	"mountain" : "It is one of the lower ones, guarding the pass."]) );
    set_exits( 
      (["west" : "/domains/Praxis/pass3",
	"southeast" : "/domains/Praxis/pass1"]));
    add_enter("cave", "/domains/Praxis/mountains/entrance");
}

