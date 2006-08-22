// coffin.c
// The outside of the coffin. It's connected by add_action to a room
// which is the inside of the coffin.

#include <mudlib.h>

#define INSIDE_COFFIN "/domains/Fooland/coffin_interior.c"
#define VAMPIRE "/domains/Fooland/vampire"
#define COFFIN_ROOM "/domains/Fooland/tower2"

inherit OBJECT ;

void create() {

	object ob ; 

	seteuid(getuid()) ;
	set ("id", ({ "coffin" }) ) ;
	set ("short", "a coffin") ;
	set ("long", "@@query_long") ;
	set ("mass", 12000) ;
	set ("bulk", 600) ;
// Load up the vampire if we need to.
	ob = find_object_or_load(VAMPIRE) ;
	ob = find_object_or_load(INSIDE_COFFIN) ;
	move(COFFIN_ROOM) ;
}

void init() {
	add_action ("enter_coffin", "enter") ;
	add_action ("look_corpse", "look") ;
}

string query_long() {

	object inside ;
	object *contents ;
	int i ;
	string str ;

	inside = find_object(INSIDE_COFFIN) ;
	if (inside && present("vampire",inside)) {
	return "It is a mahogany coffin with a purple lining.\nIt contains:\n  a pale corpse.\n" ;
	} else {
		str = "It is a mahogany coffin with a purple lining.\n" ;
		contents=all_inventory(find_object_or_load(INSIDE_COFFIN)) ;
		for (i=0;i<sizeof(contents);i++) {
			if (userp(contents[i])) {
return str+contents[i]->query("cap_name")+" is lying in it.\n" ;
			}
		}
		return str+"It is empty.\n" ;
	}
}

int enter_coffin(string str) {

	int i ;

	if (str && str!="coffin") {
		notify_fail("Enter what?\n") ;
		return 0 ;
	}
	i=this_player()->move_player(INSIDE_COFFIN,this_player()->query("cap_name")+" climbs into the coffin.") ;
	return 1 ;
}

int look_corpse (string str) {

	if (str!="at corpse") return 0 ;
	 write ("/domains/Fooland/vampire.c"->query("long")) ;
	return 1 ;
}
