// The cemetery, where ghosts come to be returned to life.
// Due to TMI's strict non-sectarian policy (and, probably, the fact that
// so many of the coders are from California) revival takes place in the
// cemetary rather than in a church or what-have-you. You can change it if
// you feel you must...
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;
inherit DOORS ;

void create () {

	object obj ;

	::create();
	set_outside("Fooland") ;
        set("light",1);
	set ("short", "A small cemetery in Footown") ;
	set ("long",
	"You are in a quiet cemetery. A statue of a long-dead adventurer is\n"+
	"standing here. It is said that if ghosts came to the statue and\n"+
	"prayed, they might be brought back to life.\n"+
	"A dark building stands to the south.\n") ;
	set ("exits", ([
	"west" : "/domains/Fooland/square",
	]) ) ;
	set ("item_desc", ([ "statue" :
	"It is a statue of Brutal, a level 19 adventurer who was killed by\n"+
	"an orc when he was 50 points away from making wizard. He prayed\n"+
	"and prayed, but he never made wizard.\n"
	]) ) ;
	create_door ("south", "north", "An iron door with a narrow slit", "locked", "silver_key") ;
	seteuid(getuid()) ;
	obj = clone_object("/obj/magic_scroll") ;
	obj->set_spell("heal") ;
	obj->move(this_object()) ;
}

void init() {
	add_action("revive_player","pray") ;
}

int revive_player() {

	int i ;
	mapping skills ;
	string *names ;

	if (!this_player()->query("ghost")) {
                notify_fail ("You don't appear to need that.\n") ;
                return 0 ;
        }
 
        this_player()->revive() ;
 
return 1; }
 
