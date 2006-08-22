// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "At the base of a dark tower.") ;
	set ("long",
	"You stand at the base of a large hill. About halfway up the slope\n"+
	"stands a tall, dark, highly ominous tower. Typing 'go tower' would\n"+
	"get you in, but you get the shakes just looking at it.\n") ;
	set ("exits", ([
	"north" : "/domains/grid/rooms/9,13.grid",
	"east" : "/domains/grid/rooms/10,14.grid",
	"west" : "/domains/grid/rooms/10,12.grid",
	"south" : "/domains/grid/rooms/11,13.grid",
	"tower" : "/domains/Fooland/tower1"
	]) ) ;
	set ("pre_exit_func", ([ "tower" : "block_players" ]) ) ;
}

int block_players() {
	if (wizardp(this_player())) return 0 ;
	write ("You start towards the base of the tower, but suddenly you are overcome\nby fear, and can progress no farther.\n") ;
	return 1 ;
}
