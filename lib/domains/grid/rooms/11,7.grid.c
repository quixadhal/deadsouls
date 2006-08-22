// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "Outside the north gates of Footown") ;
	set ("long",
	"You are standing outside the north gates of Footown. Two roads\n"+
	"lead through the town: one runs west to Smallville and the other\n"+
	"runs north, parallel to the Flowing River which is to the east.\n"+
	"South will take you inside Footown's walls.\n") ;
	set ("exits", ([
	"south" : "/domains/Fooland/nmain"
	]) ) ;
}
