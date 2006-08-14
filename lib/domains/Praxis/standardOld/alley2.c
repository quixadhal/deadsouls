#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("no castle", 1);
    set_property("light", 2);
    set_property("night light", 1);
    set_smell_string("default", "An unbelievable stench fills the air.");
    set_smell_string("dump", "It smells of all kinds of "
      "nasty rotting things.");
    set_short( "The end of a nasty alley");
    set_long(
      "This is the end of the alley. All about you can see "
"decaying buildings. Off to the south you can see an old abondoned
building, worn from the weather and a lack of care. To the north "
"there are more ruins. It appears that this used to be a whole "
"other section of Praxis, abondened long ago. The Praxis dump "
"is further west." );
       set_items(
	(["alley" : "It goes through the worst parts of Praxis.",
	  "dump" : "People take their trash there. Always "
	    "during daytime hours.",
	  "building" : "It looks like no one has paid it any "
	    "attention in eons.",
	  "ruins" : "Graffiti covers them."]) );
    set_exits( 
	      ([ "east": "/d/Praxis/alley1", 
		"north":"/d/Praxis/wall",
		"south":"/d/Praxis/building", 
		"west":"/d/Praxis/dump" ]) );
}
