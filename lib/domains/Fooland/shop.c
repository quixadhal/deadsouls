// An example shop, which inherits the standard shop object.
// All shops that inherit the standard shop should call ::create() BEFORE
// loading shop objects, because euid is set in ::create. It's easiest to
// just make ::create() the first call in the local create proc.

#include <money.h>

inherit "/std/shop" ;

void create() {
	::create() ;
	set ("light", 1) ;
        set ("short", "The village store") ;
        set ("long",
        "Welcome to the store in the little village of Footown. You can do\n"+
        "the following: list, buy, value, sell.\n") ;
	set ("exits", ([ "west" : "/domains/Fooland/smain" ]) ) ;
	storeroom = clone_object(STORAGE) ;
	ob = clone_object("/obj/shield") ;
	ob->move(storeroom) ;
	ob2 = clone_object("obj/bandages") ;
	ob2->move(storeroom) ;
}
