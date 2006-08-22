// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h> 

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "The mouth of the Flowing River.") ;
	set ("long",
	"You have reached the mouth of the Flowing River. The Flowing runs\n"+
	"down from the north and here it empties into the Bay of Wetness.\n"+
	"The beach around the bay runs to the west and south here. North\n"+
	"will take you up along the banks of the river, and some hills are in\n"+
	"the distance to the east.\n") ;
	set ("exits", ([
	"west" : "/domains/grid/rooms/12,7.grid",
	]) ) ;
}
