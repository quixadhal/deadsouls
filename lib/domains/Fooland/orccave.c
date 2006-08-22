#include <mudlib.h>

inherit ROOM ;

void create() {
	::create();
	set ("light", 0) ;
	set ("long",
"You are in a dark cave in the hills. The stench of orcs is everywhere.\n"+
"North takes you out of the cave, while south takes you deeper in.\n") ;
	set ("short", "Orc cave") ;
	set ("exits", ([
		"south" : "/domains/Fooland/orccave2.c" ,
		"north" : "/domains/grid/rooms/8,13.grid" ]) ) ;
}
