// A basic coin exchange which inherits the standard coin exchange object.
// Needs to call ::create() like the shop does.

#include <money.h>

inherit "/std/exchange" ;

void create() {
	::create() ;
        set("light",1) ;
        set ("short", "The money exchange office") ;
        set ("long",
    "You are in Footown's money exchange office. It is here you\n" +
    "may come to convert your money into gold, silver, or copper\n" +
    "coin, by typing \"convert [num] [oldtype] to [newtype]\".\n" +
    "If you don't provide a number, all of your [oldtype] coins\n" +
    "will be converted to the new currency.\n");
	set ("exits", ([ "east" : "/domains/Fooland/nmain" ]) ) ;
}
