#include <config.h>

inherit "/std/room";

void init() {
    ::init();
    add_action("any_hook", "", 1);
}

void create() {
    ::create();
    set_short( "The void");
    set_long(
      "You have been assimilated.\n"
      "From this time on, you will service the Borg.");
    set_exits( 
      (["square" : "/domains/Praxis/square"]) );
    set_property("light", 1);
}

/* patched by Valodin April 16, 1993 */
int any_hook(string str) {
    string start;

    if (start = this_player()->getenv("start"))
	this_player()->move_player(start);
    else
	this_player()->move_player("/domains/Praxis/square");
    return 0;
}

