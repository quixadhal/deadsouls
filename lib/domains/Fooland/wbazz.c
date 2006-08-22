// A street in Footown.
// Mobydick@TMI-2, 11-15-92

#include <mudlib.h>

inherit ROOM ;

void create () {
        ::create();
	set_outside("Fooland") ;
	set ("short", "Bazz Street, Footown") ;
	set ("long",
	"You are walking along the west end of Bazz Street. A large\n"+
	"mansion is to the west, and the door is open. To the north\n"+
	"is a pub, and to the south is the adventurer's guild.\n") ;
	set ("exits", ([
	"east" : "/domains/Fooland/ebazz"
	]) ) ;
}
