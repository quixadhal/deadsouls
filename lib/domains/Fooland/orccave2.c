#include <mudlib.h>

inherit ROOM ;

void create() {
	::create();
	set ("light", 0) ;
	set ("long", 
"You have come to a fork in the cavern. The stench is not so bad\n"+
	"to the southwest.\n") ;
	set ("short", "Orc cave crossroads") ;
	set ("exits", ([
		"north" : "/domains/Fooland/orccave",
		"east" : "/domains/Fooland/orccave3",
		"southwest" : "/domains/Fooland/shower" ]) ) ;
}
