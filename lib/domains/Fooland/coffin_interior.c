// coffin_interior.c
// This is where the vampire goes in the daytime.
// It actually exists in the void: it has a companion object, coffin.c,
// which appears in another room and lets you move into the coffin (if
// it's not occupied of course ;)

// Mobydick is hacking on this as of 7-14-93.

#include <mudlib.h>

#define COFFIN "/domains/Fooland/coffin.c"
#define VAMPIRE "/domains/Fooland/vampire"
#define COFFIN_ROOM "/domains/Fooland/tower2"

inherit ROOM ;

void create() {

	object ob ;

	::create() ;
	seteuid(getuid()) ;
	set ("capacity", 8000) ;
	set ("max_cap", 8000) ;
	set ("short", "Inside a coffin") ;
	set ("long", "You are inside a mahogany coffin with a purple lining. Space is rather\nrestricted.\nYou could get out of the coffin by typing \"out\".\n") ;
	ob = find_object_or_load(COFFIN) ;
	ob = find_object_or_load(VAMPIRE) ;
}

void init() {
	add_action ("leave_coffin", "out") ;
}


int leave_coffin() {

	object place ;

	place = find_object_or_load (COFFIN_ROOM) ;
	this_player()->move_player(place,"$N clambers out of the coffin.\n") ;
	return 1 ;
}
