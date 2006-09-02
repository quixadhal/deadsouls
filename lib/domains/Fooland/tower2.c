// tower2.c
// The second floor of the wizard's tower.
// The vampire lives here. He responds to time of day: at sunset he gets
// out of his coffin and starts wandering around. At sunrise, he goes back into
// the coffin.
// See the comments in all those files for more details on how this
// schtuff works.
// Written by Mobydick, 8-25-93.

#include <mudlib.h>

inherit ROOM ;

#define VAMPIRE "/domains/Fooland/vampire"
#define COFFIN "/domains/Fooland/coffin.c"

void create() {
	::create() ;
        seteuid(getuid()) ;
        set("short","A dark and musty room") ;
	set ("long",
"This room is very dark and musty, and it looks as if it is rarely\n"+
"disturbed. There are no footprints on the floor: the only place free\n"+
"of dust is by a narrow stone-edged window.\n") ;
	set ("light", 1) ;
	set ("item_desc", ([
"window" : "It is very narrow and framed with sharp-edged stones. You could\n"+
"not fit through it. Looking out it, you see the Flowing River off\n"+
"to the west, running north towards a dark forest.\n",
	]) ) ;
	find_object_or_load(COFFIN) ;
	set ("exits", ([
		"down" : "/domains/Fooland/tower1",
		"window" : "/domains/grid/9,14.grid.c"
	]) ) ;
	set ("pre_exit_func", ([ "window" : "out_the_window" ]) ) ;
}

int out_the_window() {
	if (previous_object()==VAMPIRE) {
		tell_room(this_object(),"The vampire turns into a bat, and flies out the window!\n") ;
		return 0 ;
	}
	write ("You cannot fit out the window.\n") ;
	return 1 ;
}
