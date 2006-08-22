// A street in Footown.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create();
	set_outside("Fooland") ;
	set ("short", "North Main Street, Footown") ;
	set ("long",
	"You are walking along North Main Street. The town square is to\n"+
	"the south and the town gates are to the north. East is a bank and\n"+
	"to the west is the coin exchange.\n") ;
	set ("exits", ([
	"south" : "/domains/Fooland/square",
	"north" : "/domains/grid/rooms/11,7.grid",
	]) ) ;
}
