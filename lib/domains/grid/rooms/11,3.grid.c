// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "The village of Smallville") ;
	set ("long",
	"You are in the little town of Smallville. There's not a lot here,\n"+
	"but there is a hut to the north, and there's a museum to the south\n"+
	"which has some things worth looking at.\n") ;
	set ("exits", ([
	"north" : "/domains/Fooland/hut",
	"south" : "/domains/Fooland/museum"
	]) ) ;
}
