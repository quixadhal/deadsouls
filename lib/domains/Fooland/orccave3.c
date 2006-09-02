#include <mudlib.h>

inherit ROOM ;

void create() {
        ::create();
	set ("light", 0) ;
	set ("long",
"The stench of orcs is very strong. You feel you have walked a long way \n"+
"to get here.\n") ;
	set ("short", "Orc cave.") ;
	set ("exits", ([
		"west" : "/domains/Fooland/orccave2",
		"east" : "/domains/Fooland/orccave4" ]) ) ;
}
