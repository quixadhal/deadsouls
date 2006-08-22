#include <mudlib.h>

inherit ROOM ;

void create() {
        ::create();
	set ("light", 0) ;
	set ("long",
"You are an a dark cave in the hills. The stench of orcs is everywhere.\n") ;
	set ("short", "Orc cave.") ;
	set ("exits", ([
		"west" : "/domains/Fooland/orccave3",
		"east" : "/domains/grid/rooms/9,14.grid" ]) ) ;
}
