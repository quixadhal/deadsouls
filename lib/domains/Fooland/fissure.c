#include <mudlib.h>

inherit ROOM ;

void create() {
	::create();
	set ("light", 0) ;
	set ("long",
"You stand at the bottom of a fissure in the side of the mountain. A\n"+
	"small undergound cave runs to the south.\n") ;
	set ("short", "A dark fissure") ;
	set ("exits", ([
		"south" : "/domains/Fooland/cave.c" ,
		"up" : "/domains/grid/rooms/6,2.grid" ]) ) ;
}
