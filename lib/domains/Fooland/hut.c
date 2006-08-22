#include <mudlib.h>

inherit ROOM ;

void create() {

	object obj ;

	::create();
	set ("light", 1) ;
	set ("long", 
"This is a small and pathetically dirty hut. The entire population of\n"+
"Smallville lives here.\n") ;
	set ("short", "Hut in Smallville") ;
	set ("exits", ([ "south" : "/domains/grid/rooms/11,3.grid" ]) ) ;
	seteuid(getuid()) ;
	obj = clone_object("/obj/magic_scroll") ;
	obj -> set_spell("vision") ;
	obj -> move(this_object()) ;
}
