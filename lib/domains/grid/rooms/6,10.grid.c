// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "Where the brook meets the Flowing River.") ;
	set ("long",
	"At this point a small brook flowing down from the west runs into\n"+
	"the Flowing River. Looking up the to west you can see a peaceful\n"+
	"valley that has been carved into the mountains by the brook. The\n"+
	"Flowing River continues north and south.\n") ;
	set ("exits", ([
	"north" : "/domains/grid/rooms/5,10.grid",
	"east" : "/domains/grid/rooms/6,11.grid",
	"west" : "/domains/grid/rooms/6,9.grid",
	"south" : "/domains/grid/rooms/7,10.grid",
	]) ) ;
}
