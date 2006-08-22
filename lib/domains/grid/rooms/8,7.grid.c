// A basic grid room, with non-grid exits.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>

inherit ROOM ;

void create () {
	::create() ;
	set_outside("Fooland") ;
	set ("short", "Hill view") ;
	set ("long",
	"The road climbs up a small hill here. If you typed 'view' here\n"+
	"you could get an excellent look at the area around Footown in all\n"+
	"directions.\n") ;
	set ("exits", ([
	"south" : "/domains/grid/rooms/9,7.grid",
	"north" : "/domains/grid/rooms/7,7.grid",
	"east" : "/domains/grid/rooms/8,8.grid",
	"west" : "/domains/grid/rooms/8,6.grid",
	]) ) ;
}

void init() {
	add_action ("show_view", "view") ;
}

int show_view() {
	write (
"To the south you can see the city of Footown spread out along the north\n"+
"shore of the Bay of Wetness. A long sandy beach runs along the bay shore.\n"+
"This road leads north from Footown, and a second road leads west from\n"+
"Footown to the village of Smallville. The Flowing River runs into the\n"+
"bay from the northeast, flowing more or less parallel to this road. A\n"+
"small brook passes under the road to the north, and flows into the river\n"+
"soon thereafter. The brook comes down from some high mountains off to the\n"+
"west; you can see that there is a small valley between two spines of the\n"+
"mountains. To the east is a smaller hill, and on the near slope, east and\n"+
"a little south of here is a tall, dark tower. North of here you can see\n"+
"a small forest of oak and pine trees: the road detours around it to the\n"+
"east.\n") ;
	return 1 ;
}
