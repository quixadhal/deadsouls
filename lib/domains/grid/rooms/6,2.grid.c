// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "A fissure where the brook emanates.") ;
	set ("long",
	"You are standing on the slope of the mountain. Water bubbling out\n"+
	"of a fissure here forms the brook which runs through the valley.\n"+
	"The fissure is pretty big and it looks like you could go down into\n"+
	"it. The mountain prevents you from going north, or west.\n") ;
	set ("exits", ([
	"east" : "/domains/grid/rooms/6,3.grid",
	"south" : "/domains/grid/rooms/7,2.grid",
	"down" : "/domains/Fooland/fissure"
	]) ) ;
}
