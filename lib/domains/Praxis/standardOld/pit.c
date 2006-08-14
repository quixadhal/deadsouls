//  /domains/Praxis/pit.c
//  A torture chamber, for use with the Nightmare Mortal law system.
//  Created by Manny@Nightmare 940830

#include <std.h>

inherit ROOM;

create() {
    ::create();
    set_properties( ([ "light": 0, "no teleport" : 1, "no magic" : 1
      ]) );
    set_short("Pit of Spiders");
    set_long("You are standing deep underground in "
      "a circular pit.  The walls surrounding you are completely "
      "smooth with no sign of hand holds or other ways "
      "to get out.");
    set_items( ([ ({ "walls", "wall" }) : "A smooth, circular wall, "
	"about 50 cubits deep and unnaturaly smooth." ]) );
}

reset() {
    int x;

    ::reset();
    if(!present("spider")) {
	for(x=0; x<15; ++x) {
	    new("/"+__DIR__+"obj/mon/spider")->move(this_object());
	}
    }
}
