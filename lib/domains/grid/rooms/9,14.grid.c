// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "Tall mountains, with a cave mouth.") ;
	set ("long",
	"There are tall mountains in all directions. You can't go very many\n"+
	"directions, but you can go down into an open cave mouth.\n") ;
	set ("exits", ([
	"north" : "/domains/grid/rooms/8,14.grid",
	"west" : "/domains/grid/rooms/9,13.grid",
	"down" : "/domains/Fooland/orccave4",
	]) ) ;
}
