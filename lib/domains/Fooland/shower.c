// This room is the place where the punchline of the orc stench joke
// reaches its denouement.
// Mobydick thought this was funny, once. Wrote the file 9-4-92 or so.
// I'm not sure if I want you to leave this header on or not.

#include <mudlib.h>

inherit ROOM ;

void create() {
        ::create();
	seteuid(getuid()) ;
	set ("objects", ([ "orc" : "/domains/Fooland/wet_orc" ]) ) ;
	set ("light", 1) ;
	set ("long",
"This is where the orcs come to take their showers. Water spouts\n" +
	"hang from the ceiling. Maybe that's why this place doesn't\n"+
	"smell so bad.\n") ;
	set ("short", "The orc showers.") ;
	set ("exits", ([ "northeast" : "/domains/Fooland/orccave2" ]) ) ;
	reset() ;
}
