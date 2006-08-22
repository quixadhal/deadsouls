// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "The beach at Footown") ;
	set ("long",
	"You are walking along a sandy beach near the city of Footown. To\n"+
	"the south lies the blue Bay of Wetness, and to the east the\n"+
	"Flowing River empties into the bay from the northeast. North will\n"+
	"take you into Footown proper, and the beach runs east and west.\n") ;
	set ("exits", ([
	"north" : "/domains/Fooland/smain",
	"east" : "/domains/grid/rooms/12,8.grid",
	]) ) ;
}
