// This is the post-punchline resolution of the stupid orc joke.

#include <mudlib.h>

inherit OBJECT ;

void create() {
        ::create();
	set ("id", ({ "soap" }) ) ;
	set ("short", "a bar of Ivory soap") ;
	set ("long", "Orcs love to use Ivory soap because it floats!\n") ;
	set ("mass", 25) ;
	set ("bulk", 10) ;
	set ("value", ({ 5, "copper" }) ) ;
}
