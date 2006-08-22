// A street in the town of Fooland. Rah.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create();
	set_outside("Fooland") ;
	set ("short", "Bazz Street, Footown") ;
	set ("long",
	"You are walking along Bazz Street. The town square is to\n"+
        "the east, the street continues the the west, the town\n"+
	"court house is to the north, and a post office is to the south.\n") ;
	set ("exits", ([
	"north" : "/domains/Fooland/courthouse",
	"east" : "/domains/Fooland/square",
	"west" : "/domains/Fooland/wbazz"
	]) ) ;
	set ("objects", ([ "foobird" : "/obj/foobird" ]) ) ;
	reset() ;
}
