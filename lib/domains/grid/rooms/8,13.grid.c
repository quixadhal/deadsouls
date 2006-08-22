// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "In the hills near a cave.") ;
	set ("long",
	"You are walking through rising hills. There is a small cave mouth\n"+
	"here, and a foul smell emanates from within.\n") ;
	set ("exits", ([
	"east" : "/domains/grid/rooms/8,14.grid",
	"south" : "/domains/grid/rooms/9,13.grid",
	"west" : "/domains/grid/rooms/8,12.grid",
	"north" : "/domains/grid/rooms/7,13.grid",
	"down" : "/domains/Fooland/orccave"
	]) ) ;
}
