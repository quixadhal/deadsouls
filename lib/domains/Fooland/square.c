// The town square in Footown.
// You really ought to change the inscription on the statue in this room.
// Mobydick@TMI-2, 9-3-92

#include <mudlib.h>
#include <daemons.h>

inherit ROOM ;

void create () {

	object guard ;

        ::create();
	seteuid(getuid()) ;
	guard = clone_object("/obj/town_guard.c") ;
	if(guard) guard->move(this_object()) ;
	set_outside("Fooland") ;
	set ("short", "Town square of Footown") ;
	set ("long",
	"You are in the bustling town square of Footown. There is a large\n"+
	"statue of Foo here, and a tower with a clock on top which gives\n"+
	"the time and the date. Main Street leads north and south, and Bazz\n"+
	"Street leads west. To the east is a small cemetery.\n") ;
	set ("exits", ([
	"north" : "/domains/Fooland/nmain",
	"south" : "/domains/Fooland/smain",
	"east" : "/domains/Fooland/cemetery",
	"west" : "/domains/Fooland/ebazz",
	]) ) ;
	set ("item_desc", ([ "statue" :
	"It's not any particular statue; it just means whatever statue you\n"+
	"want to have in the middle of your own town square. You REALLY ought\n"+
	"to change this.\n",
	"tower" :
	"It's about 20 feet high and made out of granite. The clock on\n"+
	"top has a nice big dial and you could \"look at clock\".\n"
	]) ) ;
	set ("item_func", ([ "clock" : "look_at_clock" ]) ) ;
}

int look_at_clock() {
	write (WEATHER_D->query_game_time()+"\n") ;
	return 1 ;
}
