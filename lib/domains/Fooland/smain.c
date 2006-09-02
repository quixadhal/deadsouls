// A street in Footown.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;
inherit DOORS ;

void create () {
        ::create();
	set_outside("Fooland") ;
	set ("short", "South Main Street, Footown") ;
	set ("long",
	"You are walking along South Main Street. The town square is to\n"+
	"the north and the beach is to the south. East is a shop and west\n"+
	"is the adventurer's hall.\n") ;
	set ("exits", ([
	"north" : "/domains/Fooland/square",
	"south" : "/domains/grid/rooms/12,7.grid",
	"east" : "/domains/Fooland/shop",
	"west" : "/domains/Fooland/hall"
	]) ) ;
	create_door ("west", "east", "A nice oak door", "closed") ;
}
