// An example bank which inherits the standard bank object. Like the shop,
// the bank should have ::create as the first call in its own create.
// This bank has a janitor. Bringing him a shrubbery solves the example
// quest in the default mudlib.

#include <money.h>
#include <move.h>

inherit "/std/bank" ;

void create() {
	::create() ;
	set ("objects", ([ "janitor" : "/domains/Fooland/janitor" ]) ) ;
	reset() ;
        set ("short", "Bank of TMI, Footown Branch") ;
        set ("long",
        "Welcome the First Mudwide Bank of TMI, Footown Branch. Here you\n"+
        "can stash your heavy extra coins so you can go get more. For a\n"+
        "fee of 25 gold, you can 'open' an account and acquire a bank card.\n"+
        "With a card you can 'deposit' and 'withdraw'.\n") ;
        set ("light", 1) ;
        set ("exits", ([
		"west" : "/domains/Fooland/nmain" ]) ) ;
}
